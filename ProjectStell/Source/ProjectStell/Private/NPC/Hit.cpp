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
		if (HitMontage != nullptr)
			Anim->PlayEnemyMontage(HitMontage);
		//요기서 경직 넣기 ?
		//그냥 그뭐냐 BT에 피격 관련 테스크를 만들어야 할듯? 체계적으로 관리 할려면?
		//HitNode->Stiff(); 이런식으로 


		break;
	case EHitEffectType::KnockBack:
		if (KnockBackMontage != nullptr)
		Anim->PlayEnemyMontage(KnockBackMontage);
		Direction = FRotationMatrix(FRotator(0, actor->GetActorRotation().Yaw, 0)).GetUnitAxis(EAxis::X);
		actor->LaunchCharacter(Direction * -1000.f, true, true);
		break;
	case EHitEffectType::Airborne:
		actor->SetActorLocation(FVector(actor->GetActorLocation().X, actor->GetActorLocation().Y, actor->GetActorLocation().Z + 1000.f));
		if (KnockDownMontage != nullptr)
		Anim->PlayEnemyMontage(KnockDownMontage);
		break;
	case EHitEffectType::KnockDown:
		if (KnockDownMontage != nullptr)
		Anim->PlayEnemyMontage(KnockDownMontage);
		break;
	case EHitEffectType::None:
		break;

	}
}

