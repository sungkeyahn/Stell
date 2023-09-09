// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/Stat.h"


UStat::UStat()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}
void UStat::InitializeComponent()
{
	Super::InitializeComponent();
	SetHp(MaxHp);
}
void UStat::InitStat(float setHp)
{
	if (setHp == -1)
		SetHp(MaxHp);
	else if (0 < setHp)
		SetHp(setHp);
}
void UStat::SetHp(float newHp)
{
	CurrentHp = newHp;
	OnHpChanged.Broadcast();
	if (CurrentHp < KINDA_SMALL_NUMBER)
	{
		CurrentHp = 0.f;
		OnHpIsZero.Broadcast();
	}
}
void UStat::SetDamage(float NewDamage)
{
	SetHp(FMath::Clamp<float>(CurrentHp - NewDamage, 0.0f, MaxHp));
}
float UStat::GetHpRatio()const
{
	if (CurrentHp<=0.f) return 0.f;
	return (CurrentHp / MaxHp);
}




