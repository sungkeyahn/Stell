// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProjectStell.h"
#include "Components/ActorComponent.h"
#include "Stat.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHpChangedDelegate);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSTELL_API UStat : public UActorComponent
{
	GENERATED_BODY()
//�ʱ�ȭ ����
public:
	UStat();
protected:
	virtual void InitializeComponent() override;
//����Ʈ ������ ���� 
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
		float MaxHp = 10;
//���� ������ ����
protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurrentHp = -1;
//������ ��ȣ�ۿ� ����
private:
	void SetHp(float newHp);
public:
	FOnHpIsZeroDelegate OnHpIsZero;
	FOnHpChangedDelegate OnHpChanged;
	void SetDamage(float NewDamage);
	float GetHpRatio()const;
	float GetMaxHp() { return MaxHp; }
	//virtual void InitStat();

};