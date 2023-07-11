// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/Enemy.h"
#include "BossA.generated.h"

//DECLARE_MULTICAST_DELEGATE(FOnSummonEndDelegate); //��ȯ ��忡 ���� ��ȣ ����

UCLASS()
class PROJECTSTELL_API ABossA : public AEnemy
{
	GENERATED_BODY()
public:
	ABossA();
protected:
	virtual void BeginPlay() override;
//��ų ���� -> ��ȯ
private:
	FTimerHandle SkillCoolTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
		float SkillCoolTime = 15.f;
	float CurCoolTime;
	void SkillCoolTimer(); //isSummonAble =true; ��Ÿ�� ���� ��ȯ ���� ���� üũ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
		TSubclassOf<AEnemy> SpawnMonsters;
	void SpawnEnemy(); //��ȯ ��ɸ� ��� -> �׽�ũ���� ȣ���ϰԵǵ�?

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* SkillMontage;
public:
	//FOnSummonEndDelegate OnSummonEnd;
	void SummonMonster(); //��ų �ִϸ��̼� ���� -> ���� -> Ż��-> �� ó�� ���� 
};
