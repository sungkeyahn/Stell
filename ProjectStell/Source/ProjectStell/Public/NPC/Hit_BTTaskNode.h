// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Hit_BTTaskNode.generated.h"


UCLASS()
class PROJECTSTELL_API UHit_BTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UHit_BTTaskNode();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecondes) override;
private:
	bool IsHit = false;
};
