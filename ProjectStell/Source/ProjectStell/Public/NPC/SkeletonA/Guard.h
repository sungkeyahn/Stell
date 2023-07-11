// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Guard.generated.h"

UCLASS()
class PROJECTSTELL_API UGuard : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UGuard();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecondes) override;
private:
	bool IsGuard = false;
};

