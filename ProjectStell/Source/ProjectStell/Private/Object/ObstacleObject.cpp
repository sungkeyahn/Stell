// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ObstacleObject.h"

#include "Stage/Section.h"

AObstacleObject::AObstacleObject()
{
	PrimaryActorTick.bCanEverTick = true;
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
}
void AObstacleObject::BeginPlay()
{
	Super::BeginPlay();

	ASection* Section = Cast<ASection>(GetOwner());
	if (Section)
		Section->OnSectionClear.AddUObject(this, &AObstacleObject::SatisfyConditions);
}
void AObstacleObject::SatisfyConditions()
{
	ASection* Section = Cast<ASection>(GetOwner());
	if (Section) DeleteObject(Section);

	Destroy();
}




