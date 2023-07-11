// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/TransparencyWall.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"

ATransparencyWall::ATransparencyWall()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TransparentMesh"));
	RootComponent = Mesh;
	Mesh->CreateDynamicMaterialInstance(0); //���̳��� ��Ƽ���� �ν��Ͻ��� ����
	Mesh->SetCollisionProfileName(TEXT("Wall"));
	TransparentTime = 0.3f;
	RunningTimer = 0.0f;
	isTransparent = false;
}
void ATransparencyWall::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.SetTickFunctionEnable(false);
}
void ATransparencyWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    RunningTimer += DeltaTime;
    float Opacity = 0.0f;
    if (isTransparent)
        Opacity = FMath::Lerp(1.0f, 0.0f, RunningTimer * (1.0f / TransparentTime));
    else
        Opacity = FMath::Lerp(0.0f, 1.0f, RunningTimer * (1.0f / TransparentTime));
    //���̳��� ��Ƽ���� �ν��Ͻ��� ��Ƽ���� �Ķ���� "Opacity"�� 0���� 1�� ����ų� 1���� 0���� ����� �۵��� ������ Tick() �Լ��� �۵��� ���߰� �Ѵ�
    Mesh->SetScalarParameterValueOnMaterials(TEXT("Opacity"), Opacity);
    if (RunningTimer > TransparentTime)
    {
        RunningTimer = 0.0f;
        PrimaryActorTick.SetTickFunctionEnable(false);
    }
}
void ATransparencyWall::SetTransparent(bool Transparent)
{
    isTransparent = Transparent;

    if (RunningTimer != 0.0f)
        RunningTimer = TransparentTime - RunningTimer;

    PrimaryActorTick.SetTickFunctionEnable(true);
}

