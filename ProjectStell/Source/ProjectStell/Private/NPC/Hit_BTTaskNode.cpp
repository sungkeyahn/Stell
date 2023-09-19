// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Hit_BTTaskNode.h"
#include "NPC/Hit.h"
#include "NPC/Enemy.h"
#include "NPC/EnemyCtrl.h"

UHit_BTTaskNode::UHit_BTTaskNode()
{
	bNotifyTick = true;
	IsHit = false;
}
void UHit_BTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecondes)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSecondes);
	if (!IsHit)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
EBTNodeResult::Type UHit_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto MonsterCharacter = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == MonsterCharacter) return EBTNodeResult::Failed;

	IsHit = true;
	UHit* hit = MonsterCharacter->hit;
	hit->OnHitEnd.AddLambda([this]() -> void { IsHit = false; });
	
	return EBTNodeResult::InProgress;
}


