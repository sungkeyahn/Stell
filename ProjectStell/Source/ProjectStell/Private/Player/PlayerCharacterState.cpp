#include "Player/PlayerCharacterState.h"
#include "StellGameInstance.h"
#include "StellSaveGame.h"

APlayerCharacterState::APlayerCharacterState()
{}
void APlayerCharacterState::Save()
{
	UStellGameInstance* gameinstance = Cast<UStellGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (gameinstance)
	{
		if (OnSave.IsBound() == true) OnSave.Broadcast();
	}
}
void APlayerCharacterState::Load()
{
	UStellGameInstance* gameinstance = Cast<UStellGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (gameinstance)
	{
		if (OnLoad.IsBound() == true) OnLoad.Broadcast();
	}
}






















