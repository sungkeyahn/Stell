// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Blueprint/UserWidget.h"
#include "WeaponQuickSlotWidget.generated.h"

UCLASS()
class PROJECTSTELL_API UWeaponQuickSlotWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
public:
		void BindCharacterWeapons(class APlayerCharacter* PC);
		void UpDateWeaponSlots();  
private:	
	TWeakObjectPtr<APlayerCharacter> pc;
	

	UPROPERTY()
		class UButton* AxeButton;
	UPROPERTY()
		class UButton* HammerButton;
	UPROPERTY()
		class UButton* ShieldButton;
	UPROPERTY()
		class UButton* SpearButton;
	UPROPERTY()
		class UButton* SwordButton;


	UFUNCTION()
		void OnAxeButtonClicked();
	UFUNCTION()
		void OnHammerButtonClicked();
	UFUNCTION()
		void OnShieldButtonClicked();
	UFUNCTION()
		void OnSpearButtonClicked();
	UFUNCTION()
		void OnSwordButtonClicked();


};
