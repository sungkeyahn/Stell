// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Components/ActorComponent.h"
#include "Weapon/Weapon.h"
#include "ComboManager.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSTELL_API UComboManager : public UActorComponent
{
	GENERATED_BODY()
public:	
	UComboManager();
	void InitComboManager();
//ĳ���� ����
private:
	class APlayerCharacter* character;
//���� ����
private:
	//FAttackInfoStruct PreAttackInfo;	
	//FAttackInfoStruct NextAttackInfo;
	FAttackInfoStruct CurrentAttackInfo;
	bool MakeAttackRange(TArray<FHitResult>* hitResults);
public:
	FAttackInfoStruct GetCurAttackInfo();
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = true))
		bool CanNextAttack;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;

//�޺� ����
public:
	void Attack(bool isLeftClick);

	bool InputCheck();
	void ConnectAttack();
	void AttackEnd();
	UFUNCTION()
		void OnAttackMontageEnded(class UAnimMontage* Montage, bool bInterrupted);
	bool FindAttackInfo(class AWeapon* clickWeapon, class AWeapon* otherWeapon);
	void AttackCheck();
	void AttackReset(); //���� ����
};
