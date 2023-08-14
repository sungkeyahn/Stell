// Fill out your copyright notice in the Description page of Project Settings.


#include "StellGameStateBase.h"
#include "ProjectStellGameModeBase.h"

AStellGameStateBase::AStellGameStateBase()
{}
void AStellGameStateBase::Save()
{
	if (OnSave.IsBound() == true) OnSave.Broadcast();
	//아래 부분을 게임을 시작할때 1번만 실행되도록 변경하기 
	AProjectStellGameModeBase* GM = Cast<AProjectStellGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GM->Save();
}
void AStellGameStateBase::Load()
{
	AProjectStellGameModeBase* GM = Cast<AProjectStellGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GM->Load();
	//위 부분을 게임을 시작할때 1번만 실행되도록 변경하기 
	if (OnLoad.IsBound() == true) OnLoad.Broadcast();
}





















