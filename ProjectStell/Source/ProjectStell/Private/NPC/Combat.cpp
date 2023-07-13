// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Combat.h"
#include "NPC/Enemy.h"
#include "NPC/EnemyAnim.h"
#include "NPC/EnemyCtrl.h"

UCombat::UCombat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

}
void UCombat::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UCombat::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//if (isAttacking)
	{
		isAttacking = false;
		OnAttackEnd.Broadcast();
	}
}
void UCombat::Attack(FEnemyAttackInfoStruct Info)
{
	if (!isAttacking)
	{
		isAttacking = true;
		curAttackInfo = Info;
		actor->GetAnim()->PlayEnemyMontage(Info.montage);
	}
}
void UCombat::AttackCheck()
{
	FHitResult hitResult;
	FCollisionQueryParams params(NAME_None, false, actor);
	bool bResult = GetWorld()->SweepSingleByChannel
	(
		hitResult, actor->GetActorLocation(),
		actor->GetActorLocation() + actor->GetActorForwardVector() * curAttackInfo.AttackRange,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(curAttackInfo.AttackRadius), params
	);

#ifdef ENABLE_DRAW_DEBUG
	FVector traceVec = actor->GetActorForwardVector() * curAttackInfo.AttackRange;
	FVector center = actor->GetActorLocation() + traceVec * 0.5f;
	float halfheight = curAttackInfo.AttackRange * 0.5f + curAttackInfo.AttackRadius;
	FQuat capsuleRot = FRotationMatrix::MakeFromZ(traceVec).ToQuat();
	FColor drawColor = bResult ? FColor::Green : FColor::Red;
	float debugLifeTime = 5.0f;
	DrawDebugCapsule(GetWorld(), center, halfheight, curAttackInfo.AttackRadius, capsuleRot, drawColor, false, debugLifeTime);
#endif 

	if (bResult)
	{
		if (hitResult.GetActor())
		{
			FDamageEvent damageEvent;
			hitResult.GetActor()->TakeDamage(curAttackInfo.Damage, damageEvent, actor->GetController(), actor);
		}
	}
}

void UCombat::Hit(FAttackInfoStruct takeAttackInfo)
{
	EEnemyState State = actor->GetState();
	UEnemyAnim* Anim = actor->GetAnim();
	if (State != EEnemyState::Idle) return;

	actor->StopUnit(takeAttackInfo.ControlTime);
	
	EHitEffectType type = takeAttackInfo.HitType;
	FVector Direction;
	switch (type)
	{
	case EHitEffectType::Stiff:
		Direction = FRotationMatrix(FRotator(0, actor->GetActorRotation().Yaw, 0)).GetUnitAxis(EAxis::X);
		actor->LaunchCharacter(Direction * -500.f, true, true);
		Anim->PlayEnemyMontage(HitMontage);
		break;
	case EHitEffectType::KnockBack:
		Direction = FRotationMatrix(FRotator(0, actor->GetActorRotation().Yaw, 0)).GetUnitAxis(EAxis::X);
		actor->LaunchCharacter(Direction * -1000.f, true, true);
		Anim->PlayEnemyMontage(HitMontage);
		break;
	case EHitEffectType::Airborne:
		actor->SetActorLocation(FVector(actor->GetActorLocation().X, actor->GetActorLocation().Y, actor->GetActorLocation().Z + 1000.f));
		Anim->PlayEnemyMontage(AirborneMontage);
		break;
	case EHitEffectType::KnockDown:
		Anim->PlayEnemyMontage(KnockDownMontage);
		break;
	case EHitEffectType::None:
		break;

	}
	
}

