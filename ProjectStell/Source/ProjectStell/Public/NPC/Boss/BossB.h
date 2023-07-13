// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/Enemy.h"
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
	//��ų ���� -> �������� skill�� �з��ؾ��ҵ� 
private:
	FTimerHandle SkillCoolTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
		float SkillCoolTime = 15.f;
	float CurCoolTime;
	void SkillCoolTimer(); //isSummonAble =true; ��Ÿ�� ���� ��ȯ ���� ���� üũ
	void Skill_JumpAttack(); //���� ���ݿ� ���� ������ ������ �ִ� �Լ� 
	UFUNCTION()
		void OnSkillEndFun(class UAnimMontage* Montage, bool bInterrupted);
protected:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
		//FEnemyAttackInfoStruct SkillAttackInfo;

public:
	FOnSkillEndDelegate OnSkillEnd;
	void Skill(); //�׽�ũ���� ȣ���� �Լ� 
};
