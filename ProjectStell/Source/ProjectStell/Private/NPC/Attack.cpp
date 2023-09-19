// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Attack.h"

#include "NPC/Enemy.h"
#include "NPC/EnemyAnim.h"
#include "NPC/EnemyCtrl.h"

UAttack::UAttack()
{}
void UAttack::BeginPlay()
{
	Super::BeginPlay();
}

void UAttack::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (isAttacking)
	{
		isAttacking = false;
		OnAttackEnd.Broadcast();
	}
}
void UAttack::Attack(FEnemyAttackInfoStruct Info)
{
	UEnemyAnim* Anim = actor->GetAnim();
	if (!isAttacking)
	{
		isAttacking = true;
		curAttackInfo = Info;
		Anim->PlayEnemyMontage(Info.montage);
	}
}	
void UAttack::AttackCheck()
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






