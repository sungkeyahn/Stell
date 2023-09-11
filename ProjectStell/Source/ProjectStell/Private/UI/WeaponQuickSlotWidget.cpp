// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WeaponQuickSlotWidget.h"
#include "Components/Button.h"
#include "Player/PlayerCharacter.h"

void UWeaponQuickSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AxeButton  = Cast<UButton>(GetWidgetFromName(TEXT("Button_0")));
	if (nullptr == AxeButton) return;
	HammerButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_1")));
	if (nullptr == HammerButton) return;
	ShieldButton  = Cast<UButton>(GetWidgetFromName(TEXT("Button_2")));
	if (nullptr == ShieldButton) return;
	SpearButton  = Cast<UButton>(GetWidgetFromName(TEXT("Button_3")));
	if (nullptr == SpearButton) return;
	SwordButton  = Cast<UButton>(GetWidgetFromName(TEXT("Button_4")));
	if (nullptr == SwordButton) return;

	SwordButton->OnClicked.AddDynamic(this, &UWeaponQuickSlotWidget::OnSwordButtonClicked);
	AxeButton->OnClicked.AddDynamic(this, &UWeaponQuickSlotWidget::OnAxeButtonClicked);
	HammerButton->OnClicked.AddDynamic(this, &UWeaponQuickSlotWidget::OnHammerButtonClicked);
	ShieldButton->OnClicked.AddDynamic(this, &UWeaponQuickSlotWidget::OnShieldButtonClicked);
	SpearButton->OnClicked.AddDynamic(this, &UWeaponQuickSlotWidget::OnSpearButtonClicked);
	
}
void UWeaponQuickSlotWidget::BindCharacterWeapons(APlayerCharacter* PC)
{
	if (nullptr == PC)return;
	pc = PC;
	PC->OnHaveWeaponChanged.AddUObject(this, &UWeaponQuickSlotWidget::UpDateWeaponSlots);
}
void UWeaponQuickSlotWidget::UpDateWeaponSlots()
{
	//Button_0->OnClicked.Add
	if (pc->UnLockWeapons.Find(0)) AxeButton->SetIsEnabled(true);
	else AxeButton->SetIsEnabled(false);
	
	if (pc->UnLockWeapons.Find(1)) HammerButton->SetIsEnabled(true);
	else HammerButton->SetIsEnabled(false);

	if (pc->UnLockWeapons.Find(2)) ShieldButton->SetIsEnabled(true);
	else ShieldButton->SetIsEnabled(false);

	if (pc->UnLockWeapons.Find(3)) SpearButton->SetIsEnabled(true);
	else SpearButton->SetIsEnabled(false);

	if (pc->UnLockWeapons.Find(4)) SwordButton->SetIsEnabled(true);
	else  SwordButton->SetIsEnabled(false);

}



void UWeaponQuickSlotWidget::OnAxeButtonClicked()
{
	if (pc->UnLockWeapons.Find(0))
		pc->SetSelectItemID(0);
}
void UWeaponQuickSlotWidget::OnHammerButtonClicked()
{
	if (pc->UnLockWeapons.Find(1))
		pc->SetSelectItemID(1);
}
void UWeaponQuickSlotWidget::OnShieldButtonClicked()
{
	if (pc->UnLockWeapons.Find(2))
		pc->SetSelectItemID(2);
}
void UWeaponQuickSlotWidget::OnSpearButtonClicked()
{
	if (pc->UnLockWeapons.Find(3))
		pc->SetSelectItemID(3);
}
void UWeaponQuickSlotWidget::OnSwordButtonClicked()
{
	if (pc->UnLockWeapons.Find(4))
		pc->SetSelectItemID(4);
}
