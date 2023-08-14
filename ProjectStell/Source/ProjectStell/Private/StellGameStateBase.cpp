// Fill out your copyright notice in the Description page of Project Settings.


#include "StellGameStateBase.h"
#include "ProjectStellGameModeBase.h"

AStellGameStateBase::AStellGameStateBase()
{}
void AStellGameStateBase::Save()
{
	if (OnSave.IsBound() == true) OnSave.Broadcast();
	//�Ʒ� �κ��� ������ �����Ҷ� 1���� ����ǵ��� �����ϱ� 
	AProjectStellGameModeBase* GM = Cast<AProjectStellGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GM->Save();
}
void AStellGameStateBase::Load()
{
	AProjectStellGameModeBase* GM = Cast<AProjectStellGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GM->Load();
	//�� �κ��� ������ �����Ҷ� 1���� ����ǵ��� �����ϱ� 
	if (OnLoad.IsBound() == true) OnLoad.Broadcast();
}





















