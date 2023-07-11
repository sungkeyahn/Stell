// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/EnemyCtrl.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NPC/Enemy.h"

const FName AEnemyCtrl::HomePosKey(TEXT("HomePos")); //������ġ
const FName AEnemyCtrl::PatrolPosKey(TEXT("PatrolPos"));
const FName AEnemyCtrl::TargetActorKey(TEXT("TargetActor"));
const FName AEnemyCtrl::isGuardKey(TEXT("Guard"));
const FName AEnemyCtrl::isSummonKey(TEXT("Summon"));
const FName AEnemyCtrl::isSkillKey(TEXT("Skill"));
AEnemyCtrl::AEnemyCtrl()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBobj(TEXT("BlackboardData'/Game/1_AI/EnemyBB.EnemyBB'"));
	if (BBobj.Succeeded())
	{
		BBAsset = BBobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTobj(TEXT("BehaviorTree'/Game/1_AI/EnemyBT.EnemyBT'"));
	if (BTobj.Succeeded())
	{
		BTAsset = BTobj.Object;
	}
}
void AEnemyCtrl::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	blackboard = Blackboard;
	if (UseBlackboard(BBAsset, blackboard))
	{
		blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset)) {}
	}
}
void AEnemyCtrl::RunBT()
{
	if (!RunBehaviorTree(BTAsset)) {}
	/*
	blackboard = Blackboard;
	if (UseBlackboard(BBAsset, blackboard))
	{
		blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset)) {}
	}*/
}
void AEnemyCtrl::StopBT()
{
	auto BTcomp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTcomp != nullptr)
		BTcomp->StopTree(EBTStopMode::Safe);
}
void AEnemyCtrl::PlayerFind() //�̰žƸ� ���̷��� ��Ʈ�ѷ��� �� ����ε� 
{
	//GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("aaaaaa")));
}
