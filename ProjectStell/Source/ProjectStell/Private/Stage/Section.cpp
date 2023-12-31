// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/Section.h"

#include "StellGameInstance.h"
#include "StellGameStateBase.h"

#include "NPC/Enemy.h"

ASection::ASection()
{
	PrimaryActorTick.bCanEverTick = true;

}
void ASection::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	auto GS = Cast<AStellGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->OnSave.AddUObject(this, &ASection::DataSaveFun);
		GS->OnLoad.AddUObject(this, &ASection::DataLoadFun);
	}
}
void ASection::BeginPlay()
{
	Super::BeginPlay();
	auto GS = Cast<AStellGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
		GS->Load();
	if (CurSectioninfo.SpawnList.IsValidIndex(0))
		for (int32 i = 0; i < CurSectioninfo.SpawnList.Num(); i++)
			Spawn(i);

}

void ASection::SectionClearConditionCheck()
{
	if (CurSectioninfo.IsSectionClear)
		if (OnSectionClear.IsBound() == true)
			OnSectionClear.Broadcast();
}

void ASection::AddSectionClearScore()
{
	CurSectioninfo.SectionClearScore += 1;
	if (SectionClearScore <= CurSectioninfo.SectionClearScore)
		CurSectioninfo.IsSectionClear = true;
	SectionClearConditionCheck();
	auto GS = Cast<AStellGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	GS->Save();
}

void ASection::Spawn(int32 index)
{
	if (CurSectioninfo.SpawnList.IsValidIndex(index))
		if (CurSectioninfo.SpawnList[index].IsDestroy == false)
		{
			FActorSpawnParameters SpawnParameter;
			SpawnParameter.Owner = this;

			AActor* spawnob = GetWorld()->SpawnActor<AActor>(CurSectioninfo.SpawnList[index].Prototype, CurSectioninfo.SpawnList[index].SpawnTransform, SpawnParameter);
			
			if (spawnob)
			{
				spawnob->SetOwner(this);

				ISpawnAble* ob = Cast<ISpawnAble>(spawnob);
				if (ob) ob->SetObjectNumber(index);

				//아래 코드를 다른 곳으로 옮기는 것에 대하여 생각해보기 
				if (CurSectioninfo.SpawnList[index].ObectType == ESpawnObectType::Monster)
				{
					if (Cast<AEnemy>(spawnob))
					{
						Cast<AEnemy>(spawnob)->OnDead.AddUObject(this, &ASection::AddSectionClearScore);
					}
				}
			}
		}
}

void ASection::DefaultSpawn(int32 index)
{
	if (DefaultSpawnList.IsValidIndex(index))
		if (DefaultSpawnList[index].IsDestroy==false)
		{
			FActorSpawnParameters SpawnParameter;
			SpawnParameter.Owner = this;

			AActor* spawnob = GetWorld()->SpawnActor<AActor>(DefaultSpawnList[index].Prototype, DefaultSpawnList[index].SpawnTransform, SpawnParameter);
			
			if (spawnob)
			{
				spawnob->SetOwner(this);

				ISpawnAble* ob = Cast<ISpawnAble>(spawnob);
				if (ob) ob->SetObjectNumber(index);

				//아래 코드를 다른 곳으로 옮기는 것에 대하여 생각해보기 
				if (DefaultSpawnList[index].ObectType == ESpawnObectType::Monster)
				{
					if (Cast<AEnemy>(spawnob))
					{
						Cast<AEnemy>(spawnob)->OnDead.AddUObject(this, &ASection::AddSectionClearScore);
					}
				}
			}
		}
}

void ASection::DataSaveFun()
{
	auto gameinstance = Cast<UStellGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	gameinstance->SaveGameInstance->Sections.Add(SectionNum, CurSectioninfo);
}
void ASection::DataLoadFun()
{
	auto gameinstance= Cast<UStellGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (gameinstance->SaveGameInstance)
	{
		FSectionStruct* SS = gameinstance->SaveGameInstance->Sections.Find(SectionNum);

		CurSectioninfo.IsSectionClear = SS->IsSectionClear;
		CurSectioninfo.SectionClearScore = SS->SectionClearScore;
		CurSectioninfo.SpawnList = SS->SpawnList;
	}
	else
	{
		CurSectioninfo.IsSectionClear = false;
		CurSectioninfo.SectionClearScore = SectionClearScore;
		CurSectioninfo.SpawnList = DefaultSpawnList;
	}
}



