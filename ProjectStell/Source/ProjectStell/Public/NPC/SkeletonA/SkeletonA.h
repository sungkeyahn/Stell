// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/Enemy.h"
#include "SkeletonA.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnGuardEndDelegate); //���� ��忡 ���� ���� ��ȣ ����

UCLASS()
class PROJECTSTELL_API ASkeletonA : public AEnemy
{
	GENERATED_BODY()
public:
	ASkeletonA();
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void WakeUp();

//���� ���� 
private:
	FTimerHandle GuardTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* GuardMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
		FMonsterAttackInfoStruct GuardAttackInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = isGuard, Meta = (AllowPrivateAccess = true))
		float GuardTime = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = isGuard, Meta = (AllowPrivateAccess = true))
		bool isGuard =false;

public:
	FOnGuardEndDelegate OnGuardEnd;
	void Guard();

};
