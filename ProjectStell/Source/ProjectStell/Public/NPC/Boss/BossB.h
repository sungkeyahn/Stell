// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/Enemy.h"
#include "NPC/Attack.h"

#include "BossB.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSkillEndDelegate);

UCLASS()
class PROJECTSTELL_API ABossB : public AEnemy
{
	GENERATED_BODY()
public:
	ABossB();
protected:
	virtual void BeginPlay() override;
private:
	FTimerHandle SkillCoolTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
		float SkillCoolTime = 15.f;
	float CurCoolTime;
	void SkillCoolTimer();
	UFUNCTION()
		void OnSkillEndFun(class UAnimMontage* Montage, bool bInterrupted);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
		FEnemyAttackInfoStruct SkillAttackInfo;
	FOnSkillEndDelegate OnSkillEnd;
	void JumpAttack();
};
