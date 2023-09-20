// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Components/ActorComponent.h"

#include "Weapon/Weapon.h"

#include "Hit.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHitEndDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSTELL_API UHit : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHit();
protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* HitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* KnockDownMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* KnockBackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit, Meta = (AllowPrivateAccess = true))
		bool isHit = false;

public:
	class AEnemy* actor;
	FOnHitEndDelegate OnHitEnd;
	UFUNCTION()
		void OnHitMontageEnded(class UAnimMontage* Montage, bool bInterrupted);
	virtual void Hit(FAttackInfoStruct takeAttackInfoe);

};
