// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/EnemyCtrl.h"
#include "BossBCtrl.generated.h"


UCLASS()
class PROJECTSTELL_API ABossBCtrl : public AEnemyCtrl
{
	GENERATED_BODY()
public:
	ABossBCtrl();
	//��ų�� Ȱ��ȭ ��Ű�� �Լ� 
	void SkillActivate(bool b);

};
