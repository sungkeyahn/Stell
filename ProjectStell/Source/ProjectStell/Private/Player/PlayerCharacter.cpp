#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterAnim.h"
#include "Player/PlayerCharaterCtrl.h"
#include "Player/ComboManager.h"
#include "Stat/PlayerStat.h"

#include "Object/Item.h"
#include "Weapon/Weapon.h"

#include "StellSaveGame.h"
#include "StellGameInstance.h"
#include "StellGameStateBase.h"
#include "Player/PlayerCharacterState.h"

#include "UI/CharacterHUDWidget.h"
#include "UI/InventoryWidget.h"
#include "UI/WeaponQuickSlotWidget.h"

#include "EngineUtils.h"




APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	//컴포넌트 생성
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Stat = CreateDefaultSubobject<UPlayerStat>(TEXT("Stat"));
	Combo = CreateDefaultSubobject<UComboManager>(TEXT("Combo"));
	
	//컴포넌트 계층구조 설정
	springArm->SetupAttachment(GetCapsuleComponent());
	camera->SetupAttachment(springArm);

	//컴포넌트 초기값 설정
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCharacter"));
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	//GetCharacterMovement()->bOrientRotationToMovement = false;
	//GetCharacterMovement()->bUseControllerDesiredRotation = true;


	//애셋 바인딩
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skin(TEXT("SkeletalMesh'/Game/MercenaryWarrior/Meshes/SK_MercenaryWarrior_WithHelmet.SK_MercenaryWarrior_WithHelmet'"));
	if (Skin.Succeeded())
		GetMesh()->SetSkeletalMesh(Skin.Object);
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/1_Player/Character/PlayerABP.PlayerABP_C'"));
	if (AnimBP.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimBP.Class);

}
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	auto PS = Cast<APlayerCharacterState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (PS)
		PS->Load();

	PlayerCtrl = Cast<APlayerCharaterCtrl>(GetController());
	if (nullptr == PlayerCtrl)return;
	PlayerCtrl->HUDWidget->BindCharacterStat(Stat);
	PlayerCtrl->InventoryWidget->BindCharacterInventory(this);
	PlayerCtrl->QuickSlotWidget->BindCharacterWeapons(this);

	if (OnHaveWeaponChanged.IsBound()) OnHaveWeaponChanged.Broadcast();

	GetWorldTimerManager().SetTimer(HPRegenerationTimerHandle, this, &APlayerCharacter::HPRegeneration, 1.0f, true);
	GetWorldTimerManager().SetTimer(SPRegenerationTimerHandle, this, &APlayerCharacter::SPRegeneration, 1.0f, true);

}
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("TurnRight"), this, &APlayerCharacter::Turn); //마우스 좌우 시점 이동
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp); //마우스 상하 시점 이동 
}
void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	auto PS = Cast<APlayerCharacterState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (PS)
	{
		PS->OnSave.AddUObject(this, &APlayerCharacter::DataSaveFun);
		PS->OnLoad.AddUObject(this, &APlayerCharacter::DataLoadFun);
	}
	
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
	DataSaveForReSpawn();
	anim->SetDeadAnim();
	SetActorEnableCollision(false);
	PlayerCtrl->ChangeInputMode(1);
	GetWorldTimerManager().ClearTimer(HPRegenerationTimerHandle);
	GetWorldTimerManager().SetTimer(CharacterDstroyTimerHandle, this, &APlayerCharacter::CharacterDestroyTimer, 1.0f, true);
	}
	);
	Combo->InitComboManager();
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
	springArm->bInheritRoll = true;
	springArm->bInheritYaw = true;

	bUseControllerRotationYaw = false;
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
	if (PlayerCtrl->isInvenopen==false)
		Combo->Attack(true);
}
void APlayerCharacter::RightAttack()
{
	if (PlayerCtrl->isInvenopen == false)
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
void APlayerCharacter::DashCoolTimer()
{
	if (Stat->GetSpRatio() >= 1.0f)
		GetWorldTimerManager().ClearTimer(DashCoolTimerHandle);

	Stat->UseStamina(-5);
	IsDashing = false;
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
	SetCanBeDamaged(true);
}

float APlayerCharacter::TakeDamage(float DamageAmout, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float FinalDamage = Super::TakeDamage(DamageAmout, DamageEvent, EventInstigator, DamageCauser);
	Stat->SetDamage(FinalDamage);
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	return FinalDamage;
}
void APlayerCharacter::HPRegeneration()
{
	CurHPRegenerationTime++;
	if (CurHPRegenerationTime >= 5.f)
	{
		CurHPRegenerationTime = 0.f;
		Stat->SetDamage(-1.f);
	}
}
void APlayerCharacter::SPRegeneration()
{
	CurSPRegenerationTime++;
	if (CurSPRegenerationTime >= 5.f)
	{
		CurSPRegenerationTime = 0.f;
		Stat->UseStamina(-50.f);
	}
}

void APlayerCharacter::Equipment_Left()
{
	if (SelectItemID == -1)return;
	if (UnLockWeapons.Find(SelectItemID))
	{
		EquipWeapon(SpawnWeapon(UnLockWeapons.Find(SelectItemID)), 0);
		SelectItemID = -1;
	}
}
void APlayerCharacter::Equipment_Right()
{
	if (SelectItemID == -1)return;
	if (UnLockWeapons.Find(SelectItemID))
	{
		EquipWeapon(SpawnWeapon(UnLockWeapons.Find(SelectItemID)), 1);
		SelectItemID = -1;
	}
}
AWeapon* APlayerCharacter::GetLeftWeapon()
{
	return leftWeapon;
}
AWeapon* APlayerCharacter::GetRightWeapon()
{
	return rightWeapon;
}

AWeapon* APlayerCharacter::SpawnWeapon(FItemInfoStruct info)
{
	auto NewWeapon = GetWorld()->SpawnActor<AWeapon>(info.ItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
	return NewWeapon;
}
AWeapon* APlayerCharacter::SpawnWeapon(FItemInfoStruct*info)
{
	auto NewWeapon = GetWorld()->SpawnActor<AWeapon>(info->ItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
	return NewWeapon;
}
void APlayerCharacter::EquipWeapon(AWeapon* newWeapon, int hand) //매개 변수를 아이템으로 바꿀 예정
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

void APlayerCharacter::ItemContacted(AItem* Item)
{
	if (Item != nullptr)
	{
		UnLockWeapons.Add(Item->GetItemInfo().ID, Item->GetItemInfo());
		if (OnHaveWeaponChanged.IsBound()) OnHaveWeaponChanged.Broadcast();
		Item->Acquiring_Item();
	}

}

void APlayerCharacter::CharacterDestroyTimer()
{
	++CharacterDstroyCoolTime;
	if (CharacterDstroyCoolTime > 3.f)
	{
		//게임 저장과 사망 UI띄우고 게임 멈추기 기능 필요
		GetWorldTimerManager().ClearTimer(CharacterDstroyTimerHandle);
		auto ps = Cast<APlayerCharacterState>(GetPlayerState());
		
		//아래 코드들은 한번만 실행해야함 타이머 코드 추가할 때 조심
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

void APlayerCharacter::DataSaveFun()
{
	const FString MapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	const FTransform Loc = GetActorTransform();
	const float hp = Stat->GetHp();
	const float sp = Stat->GetSp();
	FItemInfoStruct left = FItemInfoStruct();
	if (leftWeapon != nullptr) left = leftWeapon->Info;
	FItemInfoStruct right = FItemInfoStruct();
	if (rightWeapon != nullptr) right = rightWeapon->Info;
	UStellGameInstance* gameinstance = Cast<UStellGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	gameinstance->SaveGameInstance->PlayerStruct = FPlayerStruct(MapName, Loc, left, right, UnLockWeapons, hp, sp);
}
void APlayerCharacter::DataLoadFun()
{
	UStellGameInstance* gameinstance = Cast<UStellGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (gameinstance->SaveGameInstance->PlayerStruct.PlayerLoctions.GetLocation() != FVector::ZeroVector)
		SetActorTransform(gameinstance->SaveGameInstance->PlayerStruct.PlayerLoctions);
	if (gameinstance->SaveGameInstance->PlayerStruct.LeftWeapons.ID != -1)
		EquipWeapon(SpawnWeapon(gameinstance->SaveGameInstance->PlayerStruct.LeftWeapons), 0);
	if (gameinstance->SaveGameInstance->PlayerStruct.RightWeapons.ID != -1)
		EquipWeapon(SpawnWeapon(gameinstance->SaveGameInstance->PlayerStruct.RightWeapons), 1);
	Stat->InitStat(gameinstance->SaveGameInstance->PlayerStruct.CurHP, gameinstance->SaveGameInstance->PlayerStruct.CurSP);
	UnLockWeapons = gameinstance->SaveGameInstance->PlayerStruct.UnLockWeapons;
}

void APlayerCharacter::DataSaveForReSpawn()
{
	const FString MapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	FTransform Loc;
	for (const auto& entity:FActorRange(GetWorld()))
	{
		if (entity->GetName() == "PlayerStart")
			Loc = entity->GetTransform();
	}
	const float hp = Stat->GetMaxHp();
	const float sp = Stat->GetMaxSp();
	FItemInfoStruct left = FItemInfoStruct();
	if (leftWeapon != nullptr) left = leftWeapon->Info;
	FItemInfoStruct right = FItemInfoStruct();
	if (rightWeapon != nullptr) right = rightWeapon->Info;

	UStellGameInstance* gameinstance = Cast<UStellGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	gameinstance->SaveGameInstance->PlayerStruct = FPlayerStruct(MapName, Loc, left, right, UnLockWeapons, hp, sp);
}
