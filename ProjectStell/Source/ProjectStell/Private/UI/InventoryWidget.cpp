// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWidget.h"
#include "UI/ItemSlotWidget.h"
#include "Player/PlayerCharacter.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/*
	for (int32 i = 0; i < 24; i++)
	{
		ItemSlot.Add(Cast<UItemSlotWidget>(GetWidgetFromName(FName(FString::Printf(TEXT("ItemSlot%d"), i)))));
	}
	LeftWeapon = Cast<UItemSlotWidget>(GetWidgetFromName(TEXT("LeftWeaponSlot")));
	RightWeapon = Cast<UItemSlotWidget>(GetWidgetFromName(TEXT("RightWeaponSlot")));
	*/
	//this->WidgetTree->FindWidget<UTextBlock>("TitleTextBlock"); �̰� ���غ�
	//void AddItem(Iteminfo info); -> �� �Լ����� ������ ������ ���� ��� ���Կ� �������� ����
	//invenSlots[slotindex].SetSlot(ID, Quantity,Image);
	//LeftWeapon.SetSlot(ID, Quantity, Image);

}
void UInventoryWidget::BindCharacterInventory(APlayerCharacter* p)
{
	if (nullptr == p)return;
	p->OnInventoryChanged.AddUObject(this, &UInventoryWidget::UpdateItemSlot);
}
void UInventoryWidget::UpdateItemSlot(FItemInfoStruct info)
{
	int32 slotindex = 5;
	RightWeapon->SetSlot(info.ID, info.Quantity, info.Icon);
	//invenSlots[0]->SetSlot(info.ID, info.Quantity, info.Icon);
}
