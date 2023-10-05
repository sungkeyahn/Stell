// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GamePauseMenuWidget.h"
#include "Components/Button.h"
#include "Player/PlayerCharaterCtrl.h"
#include "Kismet/KismetSystemLibrary.h"

#include "StellSaveGame.h"
#include "StellGameInstance.h"
#include "StellGameStateBase.h"
#include "Player/PlayerCharacterState.h"

void UGamePauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (nullptr != ResumeButton)
		ResumeButton->OnClicked.AddDynamic(this, &UGamePauseMenuWidget::OnResumeClicked);

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UGamePauseMenuWidget::OnReturnToTitleClicked);

	ExitGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnExitGame")));
	if (nullptr != ExitGameButton)
		ExitGameButton->OnClicked.AddDynamic(this, &UGamePauseMenuWidget::OnExitGameClicked);
}
void UGamePauseMenuWidget::OnResumeClicked()
{
	auto playerctrl = Cast<APlayerCharaterCtrl>(GetOwningPlayer());
	if(nullptr == playerctrl)return;

	RemoveFromParent();
	playerctrl->ChangeInputMode(0);
	playerctrl->SetPause(false);
}
void UGamePauseMenuWidget::OnReturnToTitleClicked()
{
	auto PS = Cast<APlayerCharacterState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	PS->Save();
	auto GS = Cast<AStellGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	GS->Save();
	auto  gameinstance = Cast<UStellGameInstance>(GetGameInstance());
	if (gameinstance)
		gameinstance->Save();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}
void UGamePauseMenuWidget::OnExitGameClicked()
{
	auto PS = Cast<APlayerCharacterState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	PS->Save();
	auto GS = Cast<AStellGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	GS->Save();
	auto  gameinstance = Cast<UStellGameInstance>(GetGameInstance());
	if (gameinstance)
		gameinstance->Save();
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
