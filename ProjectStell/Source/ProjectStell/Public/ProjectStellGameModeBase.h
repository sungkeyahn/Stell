// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectStellGameModeBase.generated.h"

UCLASS()
class PROJECTSTELL_API AProjectStellGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
		AProjectStellGameModeBase();
		//virtual void PostInitializeComponents() override;
		//virtual void PostLogin(APlayerController* NewPlayer) override;
private:
	UPROPERTY()
		FString SaveSlotName;
	UPROPERTY()
		int32 SaveIndex;
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
