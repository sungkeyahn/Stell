// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Hit.h"

#include "NPC/Enemy.h"
#include "NPC/EnemyAnim.h"
#include "NPC/EnemyCtrl.h"

UHit::UHit()
{

}
void UHit::BeginPlay()
{
	Super::BeginPlay();
}
void UHit::OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (isHit)
	{
		isHit = false;
		OnHitEnd.Broadcast();
	}
}
void UHit::Hit(FAttackInfoStruct takeAttackInfo)
{

	EEnemyState State = actor->GetState();
	UEnemyAnim* Anim = actor->GetAnim();
	if (State != EEnemyState::Idle) return;
	
	isHit = true;
	actor->StopUnit();

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

