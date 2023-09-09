// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/Actor.h"

#include "Petten/SpawnAble.h"

#include "ObstacleObject.generated.h"

UCLASS()
class PROJECTSTELL_API AObstacleObject : public AActor, public ISpawnAble
{
	GENERATED_BODY()
	
public:	
	AObstacleObject();
protected:
	virtual void BeginPlay() override;
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	UBoxComponent* Box;

	void SatisfyConditions();
};
