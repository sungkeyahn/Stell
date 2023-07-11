// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnim.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDeletegate);
UCLASS()
class PROJECTSTELL_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
//�ʱ�ȭ �� �⺻ ���� �Լ���
public:
	UEnemyAnim();
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;
	void PlayEnemyMontage(class UAnimMontage* montage);

//���� ����
public:
	FOnAttackHitCheckDeletegate OnAttackHitCheck;
private:
	UFUNCTION()
		void AnimNotify_HitCheck();

//���� ����
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed=0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead=false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsSleep=true;
};
