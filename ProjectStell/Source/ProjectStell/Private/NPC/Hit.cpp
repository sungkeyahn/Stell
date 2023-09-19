// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Hit.h"

#include "NPC/Enemy.h"
#include "NPC/EnemyAnim.h"
#include "NPC/EnemyCtrl.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

UHit::UHit()
{}
void UHit::BeginPlay()
{
	Super::BeginPlay();
}
void UHit::HitParticleSpawn(FVector loc)
{
	if (HitParticle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),HitParticle, loc);
	}
}
void UHit::OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (isHit)
	{
		isHit = false;
		actor->GetCtrl()->TakeAttack(false);
		OnHitEnd.Broadcast();
	}
}
void UHit::Hit(FAttackInfoStruct takeAttackInfo)
{
	EEnemyState curState = actor->GetState();
	if (curState == EEnemyState::Dead) return;
	if (curState == EEnemyState::Groggy) return;
	if (curState == EEnemyState::SuperArmor) return;
	if (curState == EEnemyState::Invincibility) return;

	isHit = true;
	UEnemyAnim* Anim = actor->GetAnim();

	EHitEffectType type = takeAttackInfo.HitType;
	switch (type)
	{
	case EHitEffectType::Stiff:
		if (HitMontage != nullptr)
			Anim->PlayEnemyMontage(HitMontage);
		break;
	case EHitEffectType::KnockBack:
		if (KnockBackMontage != nullptr)
		Anim->PlayEnemyMontage(KnockBackMontage);
		actor->LaunchCharacter(FRotationMatrix(FRotator(0, actor->GetActorRotation().Yaw, 0)).GetUnitAxis(EAxis::X) * -2000.f, true, true);
		break;
	case EHitEffectType::Airborne:
		actor->SetActorLocation(FVector(actor->GetActorLocation().X, actor->GetActorLocation().Y, actor->GetActorLocation().Z + 1000.f));
		if (KnockDownMontage != nullptr)
		Anim->PlayEnemyMontage(KnockDownMontage);
		break;
	case EHitEffectType::KnockDown:
		if (KnockDownMontage != nullptr)
		Anim->PlayEnemyMontage(KnockDownMontage);
		actor->StopUnit(2.5f);
		break;
	case EHitEffectType::None:
		break;

	}
}

