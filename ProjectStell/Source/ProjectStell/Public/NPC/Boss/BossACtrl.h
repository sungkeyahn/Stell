// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/EnemyCtrl.h"
#include "BossACtrl.generated.h"

UCLASS()
class PROJECTSTELL_API ABossACtrl : public AEnemyCtrl
{
	GENERATED_BODY()
public:
	ABossACtrl();
	
	//��ȯ�����ϴٴ� ���� �����忡 �˸��� �Լ� isSummonKey���� �����ϴ� �Լ�
	void SummonAble(bool b);

};
