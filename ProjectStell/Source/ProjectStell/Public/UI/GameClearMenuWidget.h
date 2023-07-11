// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Blueprint/UserWidget.h"
#include "GameClearMenuWidget.generated.h"


UCLASS()
class PROJECTSTELL_API UGameClearMenuWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	//�� ������ ������ҵ� 
	UPROPERTY()
		class UButton* ReturnToTitleButton; //Ÿ��Ʋ�� �����ư
	UPROPERTY()
		class UButton* ExitGameButton;//���� ������ ��ư
	UFUNCTION()
		void OnReturnToTitleClicked();
	UFUNCTION()
		void OnExitGameClicked();
};
