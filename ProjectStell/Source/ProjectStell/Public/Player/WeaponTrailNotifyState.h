// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Animation/AnimNotifies/AnimNotifyState_Trail.h"
#include "WeaponTrailNotifyState.generated.h"


UCLASS()
class PROJECTSTELL_API UWeaponTrailNotifyState : public UAnimNotifyState_Trail
{
	GENERATED_BODY()
		//�÷��̾� ĳ���� ������ �մٰ� ��Ƽ���� ����ɶ� �̷��������� �ƴ����� �Ǵ��ؼ� ���� �̸� �ٲ��ָ� ��?
		//��������Ʈ�� �̷��϶� �޾Ƽ� �����̸� �ݴ�� �����ϱ�?
public:

	class UPlayerCharacterAnim* playeranim;

	UFUNCTION(BlueprintCallable)
		void SetFirstSocketName(FName name);
	UFUNCTION(BlueprintCallable)
		void SetSecondSocketName(FName name);

	
};
