// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverMenuWidget.generated.h"

UCLASS()
class PROJECTSTELL_API UGameOverMenuWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	//�� ������ ������ҵ� 
	UPROPERTY()
		class UButton* ReturnToTitleButton; //Ÿ��Ʋ�� �����ư
	UPROPERTY()
		class UButton* ReStartLevelButton; //Ÿ��Ʋ�� �����ư
	UPROPERTY()
		class UButton* ExitGameButton;//���� ������ ��ư
	UFUNCTION()
		void OnReturnToTitleClicked();
	UFUNCTION()
		void OnReStartLevelClicked();
	UFUNCTION()
		void OnExitGameClicked();
};
