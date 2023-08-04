#include "Player/PlayerCharacterState.h"
#include "ProjectStellGameModeBase.h"

APlayerCharacterState::APlayerCharacterState()
{}
void APlayerCharacterState::Save()
{
	if (OnSave.IsBound() == true) OnSave.Broadcast();
	AProjectStellGameModeBase* GM = Cast<AProjectStellGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GM->Save();
}
void APlayerCharacterState::Load()
{
	AProjectStellGameModeBase* GM = Cast<AProjectStellGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GM->Load();
	if (OnLoad.IsBound() == true) OnLoad.Broadcast();
}






















