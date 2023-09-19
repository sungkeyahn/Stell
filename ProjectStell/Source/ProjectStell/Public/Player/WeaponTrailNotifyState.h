// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Animation/AnimNotifies/AnimNotifyState_Trail.h"
#include "WeaponTrailNotifyState.generated.h"


UCLASS()
class PROJECTSTELL_API UWeaponTrailNotifyState : public UAnimNotifyState_Trail
{
	GENERATED_BODY()
		//플레이어 캐릭터 가지고 잇다가 노티파이 실행될때 미러상태인지 아닌지만 판단해서 소켓 이름 바꿔주면 끝?
		//델리게이트로 미러일때 받아서 소켓이름 반대로 설정하기?
public:

	class UPlayerCharacterAnim* playeranim;

	UFUNCTION(BlueprintCallable)
		void SetFirstSocketName(FName name);
	UFUNCTION(BlueprintCallable)
		void SetSecondSocketName(FName name);

	
};
