// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/PlayerState.h"
#include "PlayerCharacterState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateSaveDeletegate);
DECLARE_MULTICAST_DELEGATE(FOnPlayerStateLoadDeletegate);

UCLASS()
class PROJECTSTELL_API APlayerCharacterState : public APlayerState
{
	GENERATED_BODY()
public:
	APlayerCharacterState();
	FOnPlayerStateSaveDeletegate OnSave;
	FOnPlayerStateLoadDeletegate OnLoad;
private:
	void Save();
	void Load();

















};



