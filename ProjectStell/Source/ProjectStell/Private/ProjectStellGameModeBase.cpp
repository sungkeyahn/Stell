// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectStellGameModeBase.h"
#include "Player/PlayerCharaterCtrl.h"
#include "StellGameStateBase.h"
#include "Player/PlayerCharacterState.h"
#include "StellSaveGame.h"


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
	//�ε��ϴ� �κ� ���� �ʿ� 
	Load();
	Save();
}

void AProjectStellGameModeBase::Save()
{
	if (SaveGameInstance)
	{
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, SaveIndex);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SaveGameInstance is nullptr"));
	}
}
UStellSaveGame* AProjectStellGameModeBase::Load()
{
	//SaveGameInstance = Cast<UStellSaveGame>(UGameplayStatics::LoadGameFromSlot("MySaveGame", 0));
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
void AProjectStellGameModeBase::SetGameSaveSlot(FString _SaveSlotName, int32 _SaveIndex)
{
	SaveSlotName = _SaveSlotName;
	SaveIndex = _SaveIndex;
}
//������ ���۵Ǹ� UI�� ���̺� ������ �����ϵ��� �ϴ� �κе� �ݵ�� �ʿ��ҵ�?
/*
void AProjectStellGameModeBase::AddScore()
{
	StellGameState->AddGameScore();
	if (GetCurrentScore() >= StageClearScore)
	{
		StellGameState->SetGameCleared();
		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
			(*It)->TurnOff();
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			const auto pctrl = Cast<APlayerCharaterCtrl>(It->Get());
			if (nullptr != pctrl) pctrl->ShowUI_GameClear();	
		}
	}
}

int32 AProjectStellGameModeBase::GetCurrentScore() const
{
	return StellGameState->GetGameScore();
}
*/




