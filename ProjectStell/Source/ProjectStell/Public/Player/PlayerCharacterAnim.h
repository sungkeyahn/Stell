// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnim.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDeletegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndCheckDeletegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackTimeCheckDeletegate);
DECLARE_MULTICAST_DELEGATE(FOnDashStartDeletegate);
DECLARE_MULTICAST_DELEGATE(FOnDashEndDeletegate);

UCLASS()
class PROJECTSTELL_API UPlayerCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPlayerCharacterAnim();
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;
//��Ƽ���� ����
public:
	FOnAttackHitCheckDeletegate OnAttackHitCheck;
	FOnAttackEndCheckDeletegate OnAttackEndCheck;
	FOnAttackTimeCheckDeletegate OnAttackTimeCheck;
	FOnDashStartDeletegate OnDashStart;
	FOnDashEndDeletegate OnDashEnd;
private:
	UFUNCTION(BlueprintCallable)
		void AnimNotify_HitCheck();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_ConnectCheck();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_EndCheck();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_DashStart();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_DashEnd();
//ĳ���� ���� ���� 
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsMirror;
public:
	void SetMirror(bool m) { IsMirror = m; }
//��Ÿ�� ���� ����
public:
	void PlayPlayerMontage(UAnimMontage* montage, float PalySpeed = 1.0f);
	void StopPlayerMontage(UAnimMontage* montage);
	void SetDeadAnim() { IsDead = true; }
};
