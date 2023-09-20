// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/SkeletonA/SkeletonA.h"
#include "NPC/SkeletonA/SkeletonACtrl.h"

#include "NPC/EnemyAnim.h"
#include "NPC/Attack.h"
#include "NPC/Hit.h"
#include "Stat/Stat.h"

#include "ProjectStellGameModeBase.h"
#include "DrawDebugHelpers.h"

#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharaterCtrl.h"
#include "Player/ComboManager.h"

#include "Stage/Section.h"
ASkeletonA::ASkeletonA()
{}
float ASkeletonA::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (state->GetState() != EEnemyState::Dead)
	{
		FAttackInfoStruct curTakedAttackInfo = Cast<APlayerCharacter>(DamageCauser)->Combo->GetCurAttackInfo();

		if (isGuard)
		{
			GuardTime = 3.0f;
			isGuard = false;
			OnGuardEnd.Broadcast();
			GetWorldTimerManager().ClearTimer(GuardTimerHandle); 
			StopUnit(1.f);
			atk->isAttacking = false;
			atk->Attack(GuardAttackInfo); 
		}
		else
		{
			hit->Hit(curTakedAttackInfo);
			stat->SetDamage(curTakedAttackInfo.Damage);
			ctrl->TakeAttack(true);
			Cast<ASkeletonACtrl>(GetController())->Guard();
		}
	}
	return FinalDamage;
}
void ASkeletonA::WakeUp() 
{
	anim->IsSleep = false;
}
void ASkeletonA::Guard()
{
	isGuard = true;
	anim->PlayEnemyMontage(GuardMontage);
	GetWorld()->GetTimerManager().SetTimer(GuardTimerHandle, FTimerDelegate::CreateLambda([this]()->void
	{
		GuardTime -= 1.0f;
		if (GuardTime <= 0.f)
		{
			GuardTime = 3.0f;
			isGuard = false;
			OnGuardEnd.Broadcast();
			GetWorldTimerManager().ClearTimer(GuardTimerHandle);
		}
	}), 1.0f, true);
}


