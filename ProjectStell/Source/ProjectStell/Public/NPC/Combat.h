// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Components/ActorComponent.h"

#include "Weapon/Weapon.h"

#include "Combat.generated.h"


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

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSTELL_API UCombat : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombat();
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;
	
//필요한 래퍼런스들
public:
	class AEnemy* actor;


//공격 , 이동 , 피격 , 경직 등등의 전투에 관련된 행위들을 함수의 형태로 제공하는 컴포넌트

	float senseRange = 500.f;//인식 범위
	float attackSenseRange = 100.f;//공격 실행 범위
	bool isAttacking = false;//공격 실행 여부
	FEnemyAttackInfoStruct curAttackInfo; //현재 실행하고 있는 공격에 대한 정보


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
		TArray<FEnemyAttackInfoStruct> AttackList;//실행 가능한 모든 공격 정보들
	virtual void Attack(FEnemyAttackInfoStruct Info);
	//virtual void MovingAttack(FEnemyAttackInfoStruct Info,FVector position);
public:
	void AttackCheck();

	FOnAttackEndDelegate OnAttackEnd;
	UFUNCTION()
		void OnAttackMontageEnded(class UAnimMontage* Montage, bool bInterrupted);

//피격 ,경직 관련
protected:
	bool isHit = false;
	FTimerHandle SternTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* HitMontage;//피격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* KnockDownMontage;//쓰러짐
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* AirborneMontage;//공중에 뜸


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit, Meta = (AllowPrivateAccess = true))
		float SternTime = 0.0f;
public:
	void Hit(FAttackInfoStruct takeAttackInfo);
};
