// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "NPC/Enemy.h"
#include "NPC/Attack.h"
#include "SkeletonA.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnGuardEndDelegate); //가드 노드에 가드 종료 신호 전달

UCLASS()
class PROJECTSTELL_API ASkeletonA : public AEnemy
{
	GENERATED_BODY()
public:
	ASkeletonA();
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
private:
	FTimerHandle GuardTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Guard, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* GuardMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Guard, Meta = (AllowPrivateAccess = true))
		bool isGuard = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Guard, Meta = (AllowPrivateAccess = true))
		FEnemyAttackInfoStruct GuardAttackInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Guard, Meta = (AllowPrivateAccess = true))
		float GuardTime = 3.0f;

public:
	FOnGuardEndDelegate OnGuardEnd;
	void Guard();
	void WakeUp();
};
