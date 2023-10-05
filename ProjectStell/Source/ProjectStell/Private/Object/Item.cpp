// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Item.h"
#include "Stage/Section.h"

AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	movement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Movement"));
	box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = box;
	mesh->SetupAttachment(RootComponent);
}
void AItem::BeginPlay()
{
	Super::BeginPlay();
	movement->RotationRate = FRotator(0.0f, 60.f, 0.0f);
}
FItemInfoStruct AItem::GetItemInfo()
{
	return info;
}
void AItem::Acquiring_Item() //�������� ���� �� ���¿��� �������� ���� ������ ȣ��� �Լ� 
{
	//CurSpawnTime = reSpawnTime;
	//GetWorldTimerManager().SetTimer(ItemReSpawnTimmerHandle, this, &AItem::ReSpawnItemTimmer, 1.0f, true);
	//SetActorHiddenInGame(true);
	ASection* Section = Cast<ASection>(GetOwner());
	if (Section) DeleteObject(Section);

	mesh->SetVisibility(false);
	box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void AItem::ReSpawnItemTimmer()
{
	CurSpawnTime--;
	if (CurSpawnTime <=0.f)
	{
		CurSpawnTime = reSpawnTime;
		mesh->SetVisibility(true);
		box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetWorldTimerManager().ClearTimer(ItemReSpawnTimmerHandle);
	}
}

