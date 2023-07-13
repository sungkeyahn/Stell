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
	
//�ʿ��� ���۷�����
public:
	class AEnemy* actor;


//���� , �̵� , �ǰ� , ���� ����� ������ ���õ� �������� �Լ��� ���·� �����ϴ� ������Ʈ

	float senseRange = 500.f;//�ν� ����
	float attackSenseRange = 100.f;//���� ���� ����
	bool isAttacking = false;//���� ���� ����
	FEnemyAttackInfoStruct curAttackInfo; //���� �����ϰ� �ִ� ���ݿ� ���� ����


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
		TArray<FEnemyAttackInfoStruct> AttackList;//���� ������ ��� ���� ������
	virtual void Attack(FEnemyAttackInfoStruct Info);
	//virtual void MovingAttack(FEnemyAttackInfoStruct Info,FVector position);
public:
	void AttackCheck();

	FOnAttackEndDelegate OnAttackEnd;
	UFUNCTION()
		void OnAttackMontageEnded(class UAnimMontage* Montage, bool bInterrupted);

//�ǰ� ,���� ����
protected:
	bool isHit = false;
	FTimerHandle SternTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* HitMontage;//�ǰ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* KnockDownMontage;//������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* AirborneMontage;//���߿� ��


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit, Meta = (AllowPrivateAccess = true))
		float SternTime = 0.0f;
public:
	void Hit(FAttackInfoStruct takeAttackInfo);
};
