// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/PlayerStat.h"


UPlayerStat::UPlayerStat()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}
void UPlayerStat::InitializeComponent()
{
	Super::InitializeComponent();
	SetSp(MaxSp);
}
void UPlayerStat::InitStat(float setHp)
{
	Super::InitStat(setHp);
	SetSp(MaxHp);
}
void UPlayerStat::InitStat(float setHp, float setSp)
{
	Super::InitStat(setHp);
	if (setSp == -1)
		SetSp(MaxSp);
	else if (0 < setSp)
		SetSp(setSp);
}

void UPlayerStat::SetSp(float newSp)
{
	CurrentSp = newSp;
	OnSpChanged.Broadcast();
	if (CurrentSp < KINDA_SMALL_NUMBER)
		CurrentSp = 0.f;
}
void UPlayerStat::UseStamina(float NewStamina)
{
	SetSp(FMath::Clamp<float>(CurrentSp - NewStamina, 0.0f, MaxSp));
}
float UPlayerStat::GetSpRatio() const
{
	if (CurrentSp <= 0.f) return 0.f;
	return (CurrentSp / MaxSp);
}
