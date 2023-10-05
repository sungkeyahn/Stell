// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/GameModeBase.h"

#include "ProjectStellGameModeBase.generated.h"

UCLASS()
class PROJECTSTELL_API AProjectStellGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectStellGameModeBase();
	virtual void BeginPlay()override;
};
