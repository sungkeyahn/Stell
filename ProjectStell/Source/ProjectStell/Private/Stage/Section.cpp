// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/Section.h"

#include "ProjectStellGameModeBase.h"
#include "StellGameStateBase.h"


ASection::ASection()
{
	PrimaryActorTick.bCanEverTick = true;

}
void ASection::BeginPlay()
{
	Super::BeginPlay();

	auto GS=Cast<AStellGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	GS->OnSave.AddUObject(this, &ASection::DataSaveFun);
	GS->OnLoad.AddUObject(this, &ASection::DataLoadFun);

	for (int32 i = 0; i < CurSpawnList.Num(); i++)
		Spawn(i);
	
}

void ASection::Spawn(int32 index)
{
	if (!CurSpawnList[index].IsDestroy)
		AActor* spawnob = GetWorld()->SpawnActor<AActor>(CurSpawnList[index].Prototype, CurSpawnList[index].SpawnTransform);
	//스폰된 액터의 초기화는 해당 액터의 비긴플레이에서 하도록 

}

void ASection::DataSaveFun()
{
	AProjectStellGameModeBase* GM = Cast<AProjectStellGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GM->SaveGameInstance->Sections.Add(SectionNum, CurSectioninfo);
}
void ASection::DataLoadFun()
{
	AProjectStellGameModeBase* GM=Cast<AProjectStellGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	FSectionStruct* SS = GM->SaveGameInstance->Sections.Find(SectionNum);
	CurSectioninfo.IsSectionClear = SS->IsSectionClear;
	CurSectioninfo.SectionClearScore = SS->SectionClearScore;
	CurSectioninfo.SpawnList = SS->SpawnList;
}



