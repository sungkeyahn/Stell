#include "NPC/Enemy.h"
#include "NPC/EnemyAnim.h"
#include "NPC/EnemyCtrl.h"

#include "NPC/Attack.h"
#include "NPC/Hit.h"
#include "Stat/Stat.h"

#include "ProjectStellGameModeBase.h"
#include "DrawDebugHelpers.h"

#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharaterCtrl.h"
#include "Player/ComboManager.h"

#include "Stage/Section.h"

AEnemy::AEnemy()
{
	stat = CreateDefaultSubobject<UStat>(TEXT("Stat"));
	atk = CreateDefaultSubobject<UAttack>(TEXT("Attack"));
	hit = CreateDefaultSubobject<UHit>(TEXT("Hit"));
	state = new EnemyState(EEnemyState::Idle, this);

	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy")); //아직 안만듬
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	AIControllerClass = AEnemyCtrl::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	ctrl = Cast<AEnemyCtrl>(GetController());
	if (ctrl == nullptr)return;
	atk->actor = this;
	hit->actor = this;
}
void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
	if (nullptr == anim) return;
	anim->OnMontageEnded.AddDynamic(atk,&UAttack::OnAttackMontageEnded);
	anim->OnMontageEnded.AddDynamic(hit, &UHit::OnHitMontageEnded);
	

	anim->OnAttackHitCheck.AddUObject(atk, &UAttack::AttackCheck);
	stat->OnHpIsZero.AddLambda([this]()->void {state->SetState(EEnemyState::Dead); Dead(); });
}
float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (state->GetState() != EEnemyState::Dead)
	{
		ctrl->TakeAttack(true);

		FAttackInfoStruct curTakedAttackInfo = Cast<APlayerCharacter>(DamageCauser)->Combo->GetCurAttackInfo();
		hit->Hit(curTakedAttackInfo);

	
		stat->SetDamage(curTakedAttackInfo.Damage);
	}
	return FinalDamage;
}



void AEnemy::Dead()
{
	//섹션에서 오브젝트의 디스트로이를 처리할지 고민 
	ASection* Section = Cast<ASection>(GetOwner());
	if (Section) DeleteObject(Section);

	if (OnDead.IsBound() == true) OnDead.Broadcast();

	ctrl->StopBT();
	anim->IsDead = true;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void
	{
		++DeadTime;
		if (DeadTime > 3.f)
		{
			GetWorldTimerManager().ClearAllTimersForObject(this);
			//GetWorldTimerManager().ClearTimer(DeadTimerHandle);
			SetActorHiddenInGame(true);
			Destroy();
		}
	}), 1.0f, true);
}
void EnemyState::SetState(EEnemyState newState)
{
	if (curState == newState)return;
	curState = newState;
}

void AEnemy::StopUnit()
{
	//유닛 정지 타이머 하나 필요할듯?
	//GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->SetMovementMode(MOVE_None);
}
void AEnemy::StopUnit(float stopTime)
{
	curUnitStopTime = stopTime;
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	GetWorld()->GetTimerManager().SetTimer(UnitStopTimerHandle, FTimerDelegate::CreateLambda([this]()->void
	{
		curUnitStopTime -= 0.5f;
		if (curUnitStopTime <= 0.f)
		{
			GetWorldTimerManager().ClearTimer(UnitStopTimerHandle);
			RunUnit();
		}
	}), 0.5f, true);
}
void AEnemy::RunUnit()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}
