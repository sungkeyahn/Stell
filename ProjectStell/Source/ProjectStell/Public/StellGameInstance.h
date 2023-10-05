// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Engine/GameInstance.h"
#include "StellGameInstance.generated.h"

UCLASS()
class PROJECTSTELL_API UStellGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UStellGameInstance();
	virtual void Init()override;
private:
	UPROPERTY()
		FString SaveSlotName = TEXT("MySaveGame");
	UPROPERTY()
		int32 SaveIndex = 0;
public:
	UPROPERTY()
		class UStellSaveGame* SaveGameInstance;
	UFUNCTION(BlueprintCallable)
		void Save();
	UFUNCTION(BlueprintCallable)
		UStellSaveGame* Load();
	UFUNCTION(BlueprintCallable)
		void SetGameSaveSlot(FString _SaveSlotName, int32 _SaveIndex);

};
