// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Components/ActorComponent.h"
#include "Attack.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

USTRUCT(Atomic, BlueprintType)
struct FEnemyAttackInfoStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* montage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlaySpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackRange = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackRadius = 0.f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSTELL_API UAttack : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttack();
protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
		TArray<FEnemyAttackInfoStruct> AttackList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
		FEnemyAttackInfoStruct curAttackInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool isAttacking = false;

public:	
	class AEnemy* actor;
	FOnAttackEndDelegate OnAttackEnd;
	UFUNCTION()
		void OnAttackMontageEnded(class UAnimMontage* Montage, bool bInterrupted);
	virtual void Attack(FEnemyAttackInfoStruct Info);
	void AttackCheck();
};
