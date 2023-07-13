#include "Player/PlayerCharacterState.h"
#include "StellSaveGame.h"

APlayerCharacterState::APlayerCharacterState()
{
	SaveSlotName=TEXT("Player1");
}
void APlayerCharacterState::InitPlayerData()
{
	auto SaveGame = Cast<UStellSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (nullptr == SaveGame)
		SaveGame = GetMutableDefault<UStellSaveGame>();
	SavePlayerData();
	LoadPlayerData(SaveGame);
}
void APlayerCharacterState::SavePlayerData()
{
	UStellSaveGame* NewPlayerData = NewObject<UStellSaveGame>();
	NewPlayerData->PlayTime += PlayTime;
	NewPlayerData->DeadCount += DeadCount;
	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("SaveFail")));
}
void APlayerCharacterState::AddPlayTime()
{
}
void APlayerCharacterState::AddDeadCount()
{
	DeadCount += 1;
	SavePlayerData();
	/*여기서 죽음 처리*/

}
int32 APlayerCharacterState::GetDeadCount()
{
	return DeadCount;
}
void APlayerCharacterState::LoadPlayerData(UStellSaveGame* save)
{

}
