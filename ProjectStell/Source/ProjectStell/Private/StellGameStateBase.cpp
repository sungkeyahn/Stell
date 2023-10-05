// Fill out your copyright notice in the Description page of Project Settings.


#include "StellGameStateBase.h"
#include "StellGameInstance.h"

AStellGameStateBase::AStellGameStateBase()
{}
void AStellGameStateBase::Save()
{
	UStellGameInstance* gameinstance = Cast<UStellGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (gameinstance)
		if (OnSave.IsBound() == true)
			OnSave.Broadcast();
	
}
void AStellGameStateBase::Load()
{
	UStellGameInstance* gameinstance = Cast<UStellGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); 
	if (gameinstance)
		if (OnLoad.IsBound() == true)
			OnLoad.Broadcast();
}





















