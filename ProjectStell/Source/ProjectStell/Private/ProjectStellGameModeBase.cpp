// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectStellGameModeBase.h"
#include "Player/PlayerCharaterCtrl.h"
#include "Player/PlayerCharacterState.h"
#include "StellGameStateBase.h"
#include "StellGameInstance.h"


AProjectStellGameModeBase::AProjectStellGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/1_Player/Character/Player.Player_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass = APlayerCharaterCtrl::StaticClass();
	PlayerStateClass = APlayerCharacterState::StaticClass();
	GameStateClass = AStellGameStateBase::StaticClass();
}

void AProjectStellGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	auto game = Cast<UStellGameInstance>(GetGameInstance());
	game->Load();
}










