#include "NPC/Enemy.h"
#include "NPC/EnemyAnim.h"
#include "NPC/EnemyCtrl.h"

#include "NPC/Combat.h"
#include "Stat/Stat.h"

#include "ProjectStellGameModeBase.h"
#include "DrawDebugHelpers.h"

#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharaterCtrl.h"
#include "Player/ComboManager.h"

AEnemy::AEnemy()
{
	stat = CreateDefaultSubobject<UStat>(TEXT("Stat"));
	combat = CreateDefaultSubobject<UCombat>(TEXT("Combat"));
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
	
}
void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
	if (nullptr == anim) return;
	anim->OnMontageEnded.AddDynamic(combat,&UCombat::OnAttackMontageEnded);
	anim->OnAttackHitCheck.AddUObject(combat, &UCombat::AttackCheck);
	stat->OnHpIsZero.AddLambda([this]()->void {state->SetState(EEnemyState::Dead); });
	//stat->OnHpChanged.AddUObject(this, //이거 UI체크용 함수필요);
	//SetInGameState(EEnemyStateInGame::Loading);
}
float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (state->GetState() != EEnemyState::Dead)
	{
		//공격 정보
		FAttackInfoStruct curTakedAttackInfo = Cast<APlayerCharacter>(DamageCauser)->Combo->GetCurAttackInfo();
		combat->Hit(curTakedAttackInfo);

		if (curTakedAttackInfo.CameraShakeType != nullptr)
			Cast<APlayerCharaterCtrl>(EventInstigator)->PlayerCameraManager.Get()->StartCameraShake(curTakedAttackInfo.CameraShakeType, 1.0f, ECameraShakePlaySpace::CameraLocal, FRotator(0, 0, 0));

		stat->SetDamage(curTakedAttackInfo.Damage);
	}
	return FinalDamage;
}



void AEnemy::Dead()
{
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



void AEnemy::StopUnit(float stoptime)
{
	/*
	float Stoptime = stoptime;
	ctrl->StopBT();

	GetWorld()->GetTimerManager().SetTimer(StopTimerHandle, FTimerDelegate::CreateLambda([this]()->void
	{
	Stoptime -= 1.0f;
	if (Stoptime <= 0.f)
	{
		ctrl->RunBT();
		GetWorldTimerManager().ClearTimer(StopTimerHandle);
	}
	}), 1.0f, true);*/

}
