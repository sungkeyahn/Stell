// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProjectStell.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharaterCtrl.generated.h"

UCLASS()
class PROJECTSTELL_API APlayerCharaterCtrl : public APlayerController
{
	GENERATED_BODY()
public:
	APlayerCharaterCtrl();
protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

//�Է� ��� ����
private:
	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
	FInputModeGameAndUI GameAndUIInputMode;
public:
	void ChangeInputMode(int32 bGameMode = 0);

//GUI ����
public:
	bool isInvenopen = false;
	UPROPERTY()
		class UGamePauseMenuWidget* MenuWidget;
	UPROPERTY()
		class UGameClearMenuWidget* ClearWidget;
	UPROPERTY()
		class UGameOverMenuWidget* GameOverWidget;
	UPROPERTY()
		class UCharacterHUDWidget* HUDWidget;
	UPROPERTY()
		class UInventoryWidget* InventoryWidget;
	UPROPERTY()
		class UItemSlotWidget* ItemSlotWidget;
	UPROPERTY()
		class UWeaponQuickSlotWidget* QuickSlotWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ui)
		TSubclassOf<class UGamePauseMenuWidget>MenuWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ui)
		TSubclassOf<class UGameClearMenuWidget>ClearWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ui)
		TSubclassOf<class UGameOverMenuWidget>GameOverWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ui)
		TSubclassOf<class UInventoryWidget>InventoryWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ui)
		TSubclassOf<class UItemSlotWidget> ItemSlotWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ui)
		TSubclassOf<class UCharacterHUDWidget>HUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ui)
		TSubclassOf<class UWeaponQuickSlotWidget>QuickSlotWidgetClass;


	void ShowUI_GamePause();
	void ShowUI_GameClear();
	void ShowUI_GameOver();
	void ShowUI_Inventory();
	void ShowUI_QuickSlot();

	 void MoveForward(float newAxis);
	 void MoveRight(float newAxis);
	 void Evasion();
	 void RightAttack();
	 void LeftAttack();
	 void LeftEquipment();
	 void RightEquipment();
};
