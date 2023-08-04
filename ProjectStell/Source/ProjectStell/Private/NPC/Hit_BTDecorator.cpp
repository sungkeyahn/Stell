// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Hit_BTDecorator.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "NPC/Enemy.h"
#include "NPC/EnemyCtrl.h"

#include "NPC/Hit.h"

UHit_BTDecorator::UHit_BTDecorator()
{
	NodeName = TEXT("IsHit");
}

bool UHit_BTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) return false;

	bResult = ControllingPawn->hit->isHit;

	return bResult;
}
