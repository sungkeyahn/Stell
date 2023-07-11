#include "NPC/Enemy.h"
#include "NPC/EnemyAnim.h"
#include "NPC/EnemyCtrl.h"
#include "Stat/Stat.h"
#include "ProjectStellGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharaterCtrl.h"
#include "Player/ComboManager.h"

AEnemy::AEnemy()
{
	Stat = CreateDefaultSubobject<UStat>(TEXT("Stat"));
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy")); //���� �ȸ���
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	AIControllerClass = AEnemyCtrl::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	//�ʱ� ����-> "���� ����" 
	SetInGameState(EEnemyStateInGame::Init);
}
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	EnemyCtrl = Cast<AEnemyCtrl>(GetController());
	if (EnemyCtrl == nullptr)return;
	SetInGameState(EEnemyStateInGame::Ready);
}
void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
	if (nullptr == anim) return;
	anim->OnMontageEnded.AddDynamic(this, &AEnemy::OnAttackMontageEnded);
	anim->OnAttackHitCheck.AddUObject(this, &AEnemy::AttackCheck);
	Stat->OnHpIsZero.AddLambda([this]()->void {SetInGameState(EEnemyStateInGame::Dead); });
	Stat->OnHpChanged.AddUObject(this, &AEnemy::HitEffect);
	SetInGameState(EEnemyStateInGame::Loading);
}
float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (CurrentInGameState != EEnemyStateInGame::Dead)
	{
		isHit = true;
		//���� ����
		takeAttackInfo = Cast<APlayerCharacter>(DamageCauser)->Combo->GetCurAttackInfo();
		//��鸲
		if (takeAttackInfo.CameraShakeType != nullptr)
			Cast<APlayerCharaterCtrl>(EventInstigator)->PlayerCameraManager.Get()->StartCameraShake(takeAttackInfo.CameraShakeType, 1.0f, ECameraShakePlaySpace::CameraLocal, FRotator(0, 0, 0));
		//������ ����
		Stat->SetDamage(takeAttackInfo.Damage);
	}

	return FinalDamage;
}
void AEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (IsAttacking) 
	{
		IsAttacking = false;
		OnAttackEnd.Broadcast();
	}
}
void AEnemy::SetInGameState(EEnemyStateInGame newState)
{
	if (CurrentInGameState == newState)return;
	CurrentInGameState = newState;
	switch (CurrentInGameState)
	{
		case EEnemyStateInGame::Init:
		{
			SetActorHiddenInGame(true);
			SetCanBeDamaged(false);
			break;
		}
		case EEnemyStateInGame::Loading:
		{
			SetActorHiddenInGame(true);
			SetCanBeDamaged(false);
			break;
		}
		case EEnemyStateInGame::Ready:
		{
			SetActorHiddenInGame(false);
			SetCanBeDamaged(true);
			EnemyCtrl->RunBT();
			break;
		}
		case EEnemyStateInGame::Dead:
		{
			//GetCapsuleComponent()->SetSimulatePhysics(false);
			//GetCapsuleComponent()->SetActive(false);
			//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			SetCanBeDamaged(false);
			anim->IsDead=true;
			EnemyCtrl->StopBT();
			GetWorldTimerManager().ClearTimer(SternTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void
			{ 	
				++DeadTime;
				if (DeadTime > 3.f)
				{
					GetWorldTimerManager().ClearTimer(DeadTimerHandle);
					SetActorHiddenInGame(true);
					Destroy();
					auto gm = Cast<AProjectStellGameModeBase>(GetWorld()->GetAuthGameMode());
					if (nullptr != gm) gm->AddScore();
				}
			}), 1.0f, true);
			break;
		}
	}
}
void AEnemy::SetMonsterState(EMonsterState newState)
{
	if (CurrentMonsterState == newState)return;
	CurrentMonsterState = newState;
}
void AEnemy::Attack(int32 infoIndex)
{
	if (!IsAttacking)
	{
		anim->PlayEnemyMontage(AttackPatten[infoIndex].montage);
		CurrentAttackinfo = AttackPatten[infoIndex];
		IsAttacking = true;
	}
}
void AEnemy::Attack(FMonsterAttackInfoStruct attackInfo)
{
	if (!IsAttacking)
	{
		anim->PlayEnemyMontage(attackInfo.montage);
		CurrentAttackinfo = attackInfo;
		IsAttacking = true;
	}
}
void AEnemy::AttackCheck()
{
	FHitResult hitResult;
	FCollisionQueryParams params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel
	(
		hitResult,GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * CurrentAttackinfo.AttackRange,
		FQuat::Identity,ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(CurrentAttackinfo.AttackRadius),params
	);
#ifdef ENABLE_DRAW_DEBUG
	FVector traceVec = GetActorForwardVector() * CurrentAttackinfo.AttackRange;
	FVector center = GetActorLocation() + traceVec * 0.5f;
	float halfheight = CurrentAttackinfo.AttackRange * 0.5f + CurrentAttackinfo.AttackRadius;
	FQuat capsuleRot = FRotationMatrix::MakeFromZ(traceVec).ToQuat();
	FColor drawColor = bResult ? FColor::Green : FColor::Red;
	float debugLifeTime = 5.0f;
	DrawDebugCapsule(GetWorld(), center, halfheight, CurrentAttackinfo.AttackRadius, capsuleRot, drawColor, false, debugLifeTime);
#endif  
	if (bResult)
	{
		if (hitResult.GetActor())
		{
			FDamageEvent damageEvent;
			hitResult.GetActor()->TakeDamage(CurrentAttackinfo.Damage, damageEvent, GetController(), this);
		}
	}
}
void AEnemy::Stern(float sterntime) //�� �Լ��� �ð��� ������ �ڵ����� �׸�ŭ ���߰� ��������� 
{
	SternTime = sterntime;
	EnemyCtrl->StopBT();
	if (isHit) //�ǰ��߿� �Ǹ����� Ÿ�̸� �ʱ�ȭ
		GetWorldTimerManager().ClearTimer(SternTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(SternTimerHandle,FTimerDelegate::CreateLambda([this]()->void
	{
		SternTime -= 1.0f;
	if (SternTime <= 0.f)
	{
		EnemyCtrl->RunBT();
		isHit = false;
		GetWorldTimerManager().ClearTimer(SternTimerHandle);
	}}), 1.0f, true);
}
void AEnemy::HitEffect()
{
	switch (CurrentMonsterState)
	{
	case EMonsterState::Idle:
		Stern(1.0f);
		anim->PlayEnemyMontage(HitMontage);
		if (takeAttackInfo.HitType== EHitEffectType::Stiff) //���� ����
		{
			const FVector Direction = FRotationMatrix(FRotator(0, GetActorRotation().Yaw, 0)).GetUnitAxis(EAxis::X);
			LaunchCharacter(Direction * -500.f, true, true);
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("Stiff")));
		}
		else if (takeAttackInfo.HitType == EHitEffectType::KnockBack) //�˹� ����
		{
			const FVector Direction = FRotationMatrix(FRotator(0, GetActorRotation().Yaw, 0)).GetUnitAxis(EAxis::X);
			LaunchCharacter(Direction * -1000.f, true, true);
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("KnockBack")));
		}
		else if (takeAttackInfo.HitType == EHitEffectType::Airborne) //��� ����
		{
			Jump();
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("Airborne")));
		}
		break;
	case EMonsterState::SuperArmor:
		break;
	case EMonsterState::Groggy:
		//Stern(3.5f); SetMonsterState(EMonsterState::Idle); ������ ���º�ȭ�� ���⼭ �����ϴ°��̾ƴ� 
		break;
	default:
		Stern(0.0f);
		break;
	}
}

