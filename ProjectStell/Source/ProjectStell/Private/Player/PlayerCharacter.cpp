#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterAnim.h"
#include "Player/PlayerCharaterCtrl.h"
#include "Weapon/Weapon.h"
#include "Stat/PlayerStat.h"
#include "Player/ComboManager.h"
#include "Player/PlayerCharacterState.h"
#include "UI/CharacterHUDWidget.h"
#include "UI/InventoryWidget.h"
#include "DrawDebugHelpers.h"
#include "Object/Item.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	//������Ʈ ����
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Stat = CreateDefaultSubobject<UPlayerStat>(TEXT("Stat"));
	Combo = CreateDefaultSubobject<UComboManager>(TEXT("Combo"));
	
	//������Ʈ �������� ����
	springArm->SetupAttachment(GetCapsuleComponent());
	camera->SetupAttachment(springArm);

	//������Ʈ �ʱⰪ ����
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCharacter"));
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	//GetCharacterMovement()->bOrientRotationToMovement = false;
	//GetCharacterMovement()->bUseControllerDesiredRotation = true;


	//�ּ� ���ε�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skin(TEXT("SkeletalMesh'/Game/MercenaryWarrior/Meshes/SK_MercenaryWarrior_WithHelmet.SK_MercenaryWarrior_WithHelmet'"));
	if (Skin.Succeeded())
		GetMesh()->SetSkeletalMesh(Skin.Object);
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/1_Player/Character/PlayerABP.PlayerABP_C'"));
	if (AnimBP.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimBP.Class);
	DefaultViewSetting();
}
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerCtrl = Cast<APlayerCharaterCtrl>(GetController());
	if (nullptr == PlayerCtrl)return;

	PlayerCtrl->HUDWidget->BindCharacterStat(Stat);
	PlayerCtrl->InventoryWidget->BindCharacterInventory(this);

	GetWorldTimerManager().SetTimer(HPRegenerationTimerHandle, this, &APlayerCharacter::HPRegeneration, 1.0f, true);
}
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("TurnRight"), this, &APlayerCharacter::Turn); //���콺 �¿� ���� �̵�
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp); //���콺 ���� ���� �̵� 
	PlayerInputComponent->BindAction(TEXT("KillPlayer"), EInputEvent::IE_Pressed, this, &APlayerCharacter::KillPlayer);
}
void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	anim = Cast<UPlayerCharacterAnim>(GetMesh()->GetAnimInstance());
	if (anim == nullptr) return;
	anim->OnDashStart.AddLambda
	([this]()->void
	{
		IsDashing = true;
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		SetCanBeDamaged(false);
	}
	);
	anim->OnDashEnd.AddLambda
	([this]()->void
	{
		IsDashing = false;
		GetCharacterMovement()->BrakingFrictionFactor = 2.f;
		SetCanBeDamaged(true);
	}
	);
	Stat->OnHpIsZero.AddLambda
	([this]()->void
	{
	anim->SetDeadAnim();
	SetActorEnableCollision(false);
	PlayerCtrl->ChangeInputMode(1);
	GetWorldTimerManager().ClearTimer(HPRegenerationTimerHandle);
	GetWorldTimerManager().SetTimer(CharacterDstroyTimerHandle, this, &APlayerCharacter::CharacterDestroyTimer, 1.0f, true);
	}
	);
	Combo->InitComboManager();
}

void APlayerCharacter::UpDown(float NewAxisValue)
{
	if (anim->IsAnyMontagePlaying() == false)
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
	directionToMove.X = NewAxisValue;
}
void APlayerCharacter::LeftRight(float NewAxisValue)
{
	if (anim->IsAnyMontagePlaying() == false)
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
	directionToMove.Y = NewAxisValue;
}
void APlayerCharacter::LookUp(float NewAxisValue)
{
	//AddControllerPitchInput(NewAxisValue * 45.f * GetWorld()->GetDeltaSeconds());
}
void APlayerCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue * 45.f * GetWorld()->GetDeltaSeconds());
}
void APlayerCharacter::LeftAttack()
{
	Combo->Attack(true);
}
void APlayerCharacter::RightAttack()
{
	Combo->Attack(false);
}
void APlayerCharacter::Evasion()
{
	if (IsDashing || Stat->GetSpRatio() < 0.5f) return;
	Stat->UseStamina(50);
	GetWorldTimerManager().SetTimer(DashCoolTimerHandle, this, &APlayerCharacter::DashCoolTimer, 1.0f, true);
	const int32 FB = directionToMove.X;
	const int32 RL = directionToMove.Y;
	FRotator Rotation = GetControlRotation();//GetActorRotation();
	if (RL == 0 && FB > 0)
		Rotation += FRotator(0, 0, 0);
	else if (RL == 0 && FB < 0)
		Rotation += FRotator(0, 180, 0);
	else if (FB == 0 && RL > 0)
		Rotation += FRotator(0, 90, 0);
	else if (FB == 0 && RL < 0)
		Rotation += FRotator(0, -90, 0);
	else if (FB > 0 && RL > 0) //wd
		Rotation += FRotator(0, 45, 0);
	else if (FB > 0 && RL < 0) //wa
		Rotation += FRotator(0, -45, 0);
	else if (FB < 0 && RL < 0) //sa
		Rotation += FRotator(0, -135, 0);
	else if (FB < 0 && RL > 0) //sd
		Rotation += FRotator(0, 135, 0);
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	LaunchCharacter(Direction * 2000.f, true, true);

	SetActorRotation(YawRotation);
	anim->PlayPlayerMontage(DashMontage);
	Combo->AttackReset();
}

UPlayerCharacterAnim* APlayerCharacter::GetCharacterAnim()
{
	return anim;
}
void APlayerCharacter::Equipment_Left()
{
	//FItemInfoStruct ContactedItemInfo -> ������ �������� ������ �ش� ���� �����Ŵ 
	if (ContactedItem == nullptr)return;

	if (ContactedItem->GetItemInfo().ID != -1)
	{
		PutOnWeapon(ItemAcquisition(ContactedItem->GetItemInfo()), 0);
		ContactedItem->Acquiring_Item();
	}
}
void APlayerCharacter::Equipment_Right()
{
	if (ContactedItem == nullptr)return;

	if (ContactedItem->GetItemInfo().ID != -1)
	{
		PutOnWeapon(ItemAcquisition(ContactedItem->GetItemInfo()), 1);
		ContactedItem->Acquiring_Item();
	}
}
void APlayerCharacter::SetContactedItem(AItem* Item)
{
	ContactedItem = Item;
}
AWeapon* APlayerCharacter::GetLeftWeapon()
{
	return leftWeapon;
}
AWeapon* APlayerCharacter::GetRightWeapon()
{
	return rightWeapon;
}

void APlayerCharacter::HPRegeneration()
{
	CurRegenerationTime++;
	if (CurRegenerationTime >= HPRegenerationTime)
	{
		CurRegenerationTime = 0.f; 
		Stat->SetDamage(-Regeneration);
	}
}

void APlayerCharacter::DefaultViewSetting()
{

	camera->bUsePawnControlRotation = true;
	springArm->SetRelativeLocation(FVector(-60.0f, 0.0f, 0.0f));
	springArm->SetRelativeRotation(FRotator::ZeroRotator);

	springArm->bEnableCameraLag = true;
	springArm->CameraLagSpeed = 20.0;
	springArm->TargetArmLength = 450.0f;
	springArm->bUsePawnControlRotation = true;
	//springArm->bEnableCameraRotationLag = true;

	springArm->bInheritPitch = true;
	springArm->bInheritRoll = false;
	springArm->bInheritYaw = true;

	bUseControllerRotationYaw = false;
}
float APlayerCharacter::TakeDamage(float DamageAmout, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float FinalDamage = Super::TakeDamage(DamageAmout, DamageEvent, EventInstigator, DamageCauser);
	Stat->SetDamage(FinalDamage);
	UGameplayStatics::PlaySoundAtLocation(this,HitSound,GetActorLocation());
	return FinalDamage;
}
void APlayerCharacter::PutOnWeapon(AWeapon* newWeapon, int hand) //�Ű� ������ ���������� �ٲ� ����
{
	if (newWeapon == nullptr) return;
	/*UClass* BP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *path.ToString()));
	
		if (path.IsNone())
			leftWeapon = GetWorld()->SpawnActor<AWeapon>();
		else
			leftWeapon = GetWorld()->SpawnActor<AWeapon>(BP, FVector::ZeroVector, FRotator::ZeroRotator);
		leftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_lSocket"));
		leftWeapon->SetOwner(this);
	*/
	if (hand == 0)
	{
		if (leftWeapon != nullptr)
		{
			leftWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			leftWeapon->Destroy();
			leftWeapon = nullptr;
		}
		newWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, newWeapon->LeftSocketName);
		newWeapon->SetOwner(this);
		leftWeapon = newWeapon;
	}
	else 
	{
		if (rightWeapon != nullptr)
		{
			rightWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			rightWeapon->Destroy();
			rightWeapon = nullptr;
		}
		newWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, newWeapon->RightSocketName);
		newWeapon->SetOwner(this);
		rightWeapon = newWeapon;
	}
}
void APlayerCharacter::DashCoolTimer()
{
	if (Stat->GetSpRatio() >= 1.0f)
		GetWorldTimerManager().ClearTimer(DashCoolTimerHandle);

	Stat->UseStamina(-5);
	IsDashing = false;
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
	SetCanBeDamaged(true);
}
void APlayerCharacter::CharacterDestroyTimer()
{
	++CharacterDstroyCoolTime;
	if (CharacterDstroyCoolTime > 3.f)
	{
		//���� ����� ��� UI���� ���� ���߱� ��� �ʿ�
		GetWorldTimerManager().ClearTimer(CharacterDstroyTimerHandle);
		auto ps = Cast<APlayerCharacterState>(GetPlayerState());
		if (nullptr != ps) ps->AddDeadCount();
		//�Ʒ� �ڵ���� �ѹ��� �����ؾ��� Ÿ�̸� �ڵ� �߰��� �� ����
		if (leftWeapon!=nullptr)
		{
			leftWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			leftWeapon->Destroy();
		}
		if (rightWeapon != nullptr)
		{
			rightWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			rightWeapon->Destroy();
		}
		leftWeapon = nullptr;
		rightWeapon = nullptr;
		SetActorHiddenInGame(true);
		
		PlayerCtrl->ShowUI_GameOver();
	}
}
void APlayerCharacter::LoadInvenData()
{
}
bool APlayerCharacter::AddItem(FItemInfoStruct info)
{//�� �Լ������� �κ��� �������� ���� ������ ���������� �˻�
	bool result =true;
	if (Inventory.Find(info.ID))
	{
		Inventory.Find(info.ID)->Quantity += info.Quantity;
		OnInventoryChanged.Broadcast(info);
	}
	else if (info.ID > 0)
	{
		Inventory.Add(info.ID, info);
		OnInventoryChanged.Broadcast(info);
	}
	else
		result = false;

	/*���⼭ �κ�UI�� ���� ���� �������� ������ �ִ� �۾��� �ʿ�
	* ��Ʈ�ѷ��� �κ� UI�� �ǵ鿩�� UPdateInvenUI()�� �����Ű�� �������� ����*/
	
	
	
	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("Add Item %d"), Inventory.Find(info.ID)->Quantity));
	return result;
}
FItemInfoStruct* APlayerCharacter::GetItem(int32 ID)
{
	return Inventory.Find(ID);
}
TMap<int32, FItemInfoStruct> APlayerCharacter::GetInventory()
{
	return Inventory;
}
AWeapon* APlayerCharacter::ItemAcquisition(FItemInfoStruct info)
{	
	/*�� �Լ������� ���������� ���忡 �����ϴ� ������ ������ �˻�
	if (AddItem(info)) return true;
	else return false;*/
	auto NewWeapon = GetWorld()->SpawnActor<AWeapon>(info.ItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
	/*
	if (info.Type== EItemType::Weapon)
	{
		auto NewWeapon = GetWorld()->SpawnActor<AWeapon>(info.ItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (leftWeapon == nullptr && rightWeapon == nullptr)
			PutOnWeapon(NewWeapon, 0);
		else if (leftWeapon != nullptr && rightWeapon == nullptr)
			PutOnWeapon(NewWeapon, 1);
		else if (leftWeapon != nullptr && rightWeapon != nullptr)
			PutOnWeapon(NewWeapon, 0);
	}
	*/
	return NewWeapon;
}
void APlayerCharacter::KillPlayer()
{
	Stat->SetDamage(100.f);
}
