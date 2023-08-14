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

	hit->OnHitEnd.AddUObject(this, &AEnemy::RunUnit);
	//stat->OnHpChanged.AddUObject(this, //이거 UI갱신 용 함수필요);
	//SetInGameState(EEnemyStateInGame::Loading);
}
float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (state->GetState() != EEnemyState::Dead)
	{
		//공격 정보
		FAttackInfoStruct curTakedAttackInfo = Cast<APlayerCharacter>(DamageCauser)->Combo->GetCurAttackInfo();
		hit->Hit(curTakedAttackInfo);

		if (curTakedAttackInfo.CameraShakeType != nullptr)
			Cast<APlayerCharaterCtrl>(EventInstigator)->PlayerCameraManager.Get()->StartCameraShake(curTakedAttackInfo.CameraShakeType, 1.0f, ECameraShakePlaySpace::CameraLocal, FRotator(0, 0, 0));

		stat->SetDamage(curTakedAttackInfo.Damage);
	}
	return FinalDamage;
}



void AEnemy::Dead()
{
	if (OnDead.IsBound() == true)
		OnDead.Broadcast();
	ASection* Section = Cast<ASection>(GetOwner());
	if (Section)
	{
		Section->AddSectionClearScore(1);
		if (Section->CurSectioninfo.SpawnList.IsValidIndex(Num))
			Section->CurSectioninfo.SpawnList[Num].IsDestroy = true;
	}


	ctrl->StopBT();
	anim->IsDead = true;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void
	{
		++DeadTime;
		if (DeadTime > 3.f)
		{
			GetWorldTimerManager().ClearTimer(DeadTimerHandle);
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
	//ctrl->StopBT();
	anim->StopAllMontages(0.15f);
	GetCharacterMovement()->StopMovementImmediately();
}

void AEnemy::RunUnit()
{
	//ctrl->RunBT();
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}
