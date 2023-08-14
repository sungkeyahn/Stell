#include "Player/PlayerCharacterState.h"
#include "ProjectStellGameModeBase.h"
#include "StellSaveGame.h"

APlayerCharacterState::APlayerCharacterState()
{}
void APlayerCharacterState::Save()
{
	AProjectStellGameModeBase* GM = Cast<AProjectStellGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM->SaveGameInstance)
	{
		if (OnSave.IsBound() == true) OnSave.Broadcast();
	}
}
void APlayerCharacterState::Load()
{
	AProjectStellGameModeBase* GM = Cast<AProjectStellGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM->SaveGameInstance)
	{
		if (OnLoad.IsBound() == true) OnLoad.Broadcast();
	}
}






















