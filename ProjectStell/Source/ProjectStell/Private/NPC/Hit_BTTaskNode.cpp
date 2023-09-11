// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Hit_BTTaskNode.h"
#include "NPC/Hit.h"
#include "NPC/Enemy.h"
#include "NPC/EnemyCtrl.h"

UHit_BTTaskNode::UHit_BTTaskNode()
{
}

EBTNodeResult::Type UHit_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type();
}

void UHit_BTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecondes)
{

}
