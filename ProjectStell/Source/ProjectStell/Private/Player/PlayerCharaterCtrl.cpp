// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharaterCtrl.h"

#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterAnim.h"
#include "Stat/PlayerStat.h"


#include "UI/GamePauseMenuWidget.h"
#include "UI/GameClearMenuWidget.h"
#include "UI/GameOverMenuWidget.h"
#include "UI/CharacterHUDWidget.h"
#include "UI/InventoryWidget.h"
#include "UI/ItemSlotWidget.h"
#include "UI/WeaponQuickSlotWidget.h"

#include "Petten/Command.h"



APlayerCharaterCtrl::APlayerCharaterCtrl()
{
	static ConstructorHelpers::FClassFinder<UGamePauseMenuWidget> MENU(TEXT("WidgetBlueprint'/Game/1_UI/UI_Menu.UI_Menu_C'"));
	if (MENU.Succeeded()) MenuWidgetClass = MENU.Class;

	static ConstructorHelpers::FClassFinder<UGameClearMenuWidget> UI(TEXT("WidgetBlueprint'/Game/1_UI/UI_Clear.UI_Clear_C'"));
	if (UI.Succeeded()) ClearWidgetClass = UI.Class;

	static ConstructorHelpers::FClassFinder<UGameOverMenuWidget> GameOverUI(TEXT("WidgetBlueprint'/Game/1_UI/UI_GameOver.UI_GameOver_C'"));
	if (GameOverUI.Succeeded()) GameOverWidgetClass = GameOverUI.Class;
	
	static ConstructorHelpers::FClassFinder<UInventoryWidget> invenUI(TEXT("WidgetBlueprint'/Game/1_UI/Inventory.Inventory_C'"));
	if (invenUI.Succeeded()) InventoryWidgetClass = invenUI.Class;

	static ConstructorHelpers::FClassFinder<UItemSlotWidget> itemSlotUI(TEXT("WidgetBlueprint'/Game/1_UI/InvenSlot.InvenSlot_C'"));
	if (itemSlotUI.Succeeded()) ItemSlotWidgetClass = itemSlotUI.Class;
	
	static ConstructorHelpers::FClassFinder<UCharacterHUDWidget> HUDUI(TEXT("WidgetBlueprint'/Game/1_UI/PlayerHUD.PlayerHUD_C'"));
	if (HUDUI.Succeeded()) HUDWidgetClass = HUDUI.Class;

	static ConstructorHelpers::FClassFinder<UWeaponQuickSlotWidget> QuickSlot(TEXT("WidgetBlueprint'/Game/1_UI/QuickSlotHUD.QuickSlotHUD_C'"));
	if (QuickSlot.Succeeded()) QuickSlotWidgetClass = QuickSlot.Class;


}
void APlayerCharaterCtrl::SetupInputComponent()
{
	Super::SetupInputComponent();
	/**/
	InputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharaterCtrl::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharaterCtrl::MoveRight);
	InputComponent->BindAction(TEXT("Evasion"), EInputEvent::IE_Pressed, this, &APlayerCharaterCtrl::Evasion);
	InputComponent->BindAction(TEXT("LeftAttack"), EInputEvent::IE_Pressed, this, &APlayerCharaterCtrl::LeftAttack);
	InputComponent->BindAction(TEXT("RightAttack"), EInputEvent::IE_Pressed, this, &APlayerCharaterCtrl::RightAttack);
	InputComponent->BindAction(TEXT("Equipment_Left"), EInputEvent::IE_Pressed, this, &APlayerCharaterCtrl::LeftEquipment);
	InputComponent->BindAction(TEXT("Equipment_Right"), EInputEvent::IE_Pressed, this, &APlayerCharaterCtrl::RightEquipment);
	
	InputComponent->BindAction(TEXT("GamePause"),EInputEvent::IE_Pressed,this, &APlayerCharaterCtrl::ShowUI_GamePause);
	InputComponent->BindAction(TEXT("QuickSlotOpen"), EInputEvent::IE_Pressed, this, &APlayerCharaterCtrl::ShowUI_QuickSlot);

}
void APlayerCharaterCtrl::BeginPlay()
{
	Super::BeginPlay();
	ChangeInputMode(0);

	HUDWidget = CreateWidget<UCharacterHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();
	
	InventoryWidget = CreateWidget<UInventoryWidget>(this, InventoryWidgetClass);
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	
	QuickSlotWidget = CreateWidget<UWeaponQuickSlotWidget>(this, QuickSlotWidgetClass);
	QuickSlotWidget->AddToViewport();
	QuickSlotWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void APlayerCharaterCtrl::ChangeInputMode(int32 bGameMode)
{
	if (bGameMode==0)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else if (bGameMode == 1)
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
	else if (bGameMode == 2)
	{
		SetInputMode(GameAndUIInputMode);
		bShowMouseCursor = true;
	}
}
void APlayerCharaterCtrl::ShowUI_GamePause()
{
/*
ClearWidget = CreateWidget<UGameClearMenuWidget>(this, ClearWidgetClass);
if (nullptr == ClearWidget)return;
ClearWidget->AddToViewport();
ChangeInputMode(1);
*/
	ShowGUICommand* showGUI = new ShowGUICommand(this, 0);
	showGUI->Execute();
}
void APlayerCharaterCtrl::ShowUI_GameClear()
{
	TUniquePtr<ShowGUICommand> showGUI = TUniquePtr<ShowGUICommand>(new ShowGUICommand(this, 1));
	//ShowGUICommand* showGUI = new ShowGUICommand(this, 1);
	showGUI->Execute();
}
void APlayerCharaterCtrl::ShowUI_GameOver()
{
	TUniquePtr<ShowGUICommand> showGUI = TUniquePtr<ShowGUICommand>(new ShowGUICommand(this, 2));
	//ShowGUICommand* showGUI = new ShowGUICommand(this, 2);
	showGUI->Execute();
}
void APlayerCharaterCtrl::ShowUI_Inventory() //인벤 보여주기 인벤 숨기기 기능이 합쳐진 함수임으로 개편이 필요 
{
	TUniquePtr<ShowGUICommand> showGUI = TUniquePtr<ShowGUICommand>(new ShowGUICommand(this, 3));
	//ShowGUICommand* showGUI = new ShowGUICommand(this, 3);
	showGUI->Execute();
}
void APlayerCharaterCtrl::ShowUI_QuickSlot()
{
	if (nullptr == QuickSlotWidget)return;
	if (isInvenopen)
	{
		QuickSlotWidget->SetVisibility(ESlateVisibility::Collapsed);
		ChangeInputMode(0);
		isInvenopen = false;
	}
	else
	{
		QuickSlotWidget->SetVisibility(ESlateVisibility::Visible);
		ChangeInputMode(2);
		isInvenopen = true;
	}
}

void APlayerCharaterCtrl::MoveForward(float newAxis)
{
	TUniquePtr<MoveCommand> move = TUniquePtr<MoveCommand>(new MoveCommand(GetCharacter(), newAxis, true));
	//MoveCommand* move = new MoveCommand(GetCharacter(), newAxis, true);
	move->Execute();
}
void APlayerCharaterCtrl::MoveRight(float newAxis)
{
	TUniquePtr<MoveCommand> move = TUniquePtr<MoveCommand>(new MoveCommand(GetCharacter(), newAxis, false));
	//MoveCommand* move = new MoveCommand(GetCharacter(), newAxis, false);
	move->Execute();
}
void APlayerCharaterCtrl::Evasion()
{
	TUniquePtr<EvasionCommand> evasion = TUniquePtr<EvasionCommand>(new EvasionCommand(GetCharacter()));
	//EvasionCommand* evasion = new EvasionCommand(GetCharacter());
	evasion->Execute();
}
void APlayerCharaterCtrl::LeftAttack()
{
	TUniquePtr<AttackCommand> attack = TUniquePtr<AttackCommand>(new AttackCommand(GetCharacter(), true));
	//AttackCommand* attack = new AttackCommand(GetCharacter(), true);
	attack->Execute();
}
void APlayerCharaterCtrl::RightAttack()
{
	TUniquePtr<AttackCommand> attack = TUniquePtr<AttackCommand>(new AttackCommand(GetCharacter(), false));
	//AttackCommand* attack = new AttackCommand(GetCharacter(),false);
	attack->Execute();
}
void APlayerCharaterCtrl::LeftEquipment()
{
	TUniquePtr<EquipmentCommand> equipment = TUniquePtr<EquipmentCommand>(new EquipmentCommand(GetCharacter(), true));
	//EquipmentCommand *equipment = new EquipmentCommand(GetCharacter(), true);
	equipment->Execute();
}
void APlayerCharaterCtrl::RightEquipment()
{
	TUniquePtr<EquipmentCommand> equipment = TUniquePtr<EquipmentCommand>(new EquipmentCommand(GetCharacter(), false));
	//EquipmentCommand* equipment = new EquipmentCommand(GetCharacter(), false);
	equipment->Execute();
}

//백업용 코드
/*
void APlayerCharaterCtrl::OpenMainMenu()
{
	ShowGUICommand showGUI = ShowGUICommand(GetCharacter(),0);
	showGUI.Execute();

	MenuWidget = CreateWidget<UGamePauseMenuWidget>(this, MenuWidgetClass);
	if (nullptr == MenuWidget)return;
	MenuWidget->AddToViewport(3);
	SetPause(true);
	ChangeInputMode(1);
}
void APlayerCharaterCtrl::OpenInventory()
{
	ShowGUICommand showGUI = ShowGUICommand(GetCharacter(), 1);
	showGUI.Execute();

	if (isInvenopen)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		ChangeInputMode(0);
		isInvenopen = false;
	}
	else
	{
		if (nullptr == InventoryWidget)return;
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		ChangeInputMode(2);
		isInvenopen = true;
	}
}
*/