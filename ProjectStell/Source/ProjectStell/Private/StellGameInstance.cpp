// Fill out your copyright notice in the Description page of Project Settings.


#include "StellGameInstance.h"

#include "StellSaveGame.h"

#include "Player/PlayerCharacterState.h"
#include "StellGameStateBase.h"

UStellGameInstance::UStellGameInstance()
{}

void UStellGameInstance::Init()
{
	Super::Init();
}

void UStellGameInstance::Save()
{
	if (SaveGameInstance)
	{
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, SaveIndex);
	}
	else
	{
		SaveGameInstance = Cast<UStellSaveGame>(UGameplayStatics::CreateSaveGameObject(UStellSaveGame::StaticClass()));
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, SaveIndex); 
		UE_LOG(LogTemp, Error, TEXT("SaveGameInstance is nullptr"));
	}
}

UStellSaveGame* UStellGameInstance::Load()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveIndex))
	{
		SaveGameInstance = Cast<UStellSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveIndex));
	}
	else
	{
		SaveGameInstance = Cast<UStellSaveGame>(UGameplayStatics::CreateSaveGameObject(UStellSaveGame::StaticClass())); \
	}
	 
	return SaveGameInstance;
}

void UStellGameInstance::SetGameSaveSlot(FString _SaveSlotName, int32 _SaveIndex)
{
	SaveSlotName = _SaveSlotName;
	SaveIndex = _SaveIndex;
}
