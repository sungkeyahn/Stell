// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Blueprint/UserWidget.h"
#include "GamePauseMenuWidget.generated.h"

UCLASS()
class PROJECTSTELL_API UGamePauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
//�� ������ ������ҵ� 
	UPROPERTY()
		class UButton* ResumeButton; //���ư��� ��ư
	UPROPERTY()
		class UButton* ReturnToTitleButton; //Ÿ��Ʋ�� �����ư
	UPROPERTY()
		class UButton* ExitGameButton;//���� ������ ��ư
	UFUNCTION()
		void OnResumeClicked();
	UFUNCTION()
		void OnReturnToTitleClicked();
	UFUNCTION()
		void OnExitGameClicked();
};
