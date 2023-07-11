// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/Actor.h"
#include "TransparencyWall.generated.h"
/*
1. ī�޶󿡼� ĳ���ͷ� ������ �� or ĳ���� �� ī�޶� ���̿� �ݸ����� ��ġ
2. ���� ī�޶󿡼� ĳ���ͷ� ��� �������� "��(����ȭ���ɾ���)"�̶�� ��ä�� ����� 
3. �ش� ���� ����ȭ ����� Ȱ��ȭ 
*/
UCLASS()
class PROJECTSTELL_API ATransparencyWall : public AActor
{
	GENERATED_BODY()
public:	
	ATransparencyWall();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
        class UStaticMeshComponent* Mesh;
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
        float TransparentTime;
    bool isTransparent;
    float RunningTimer; //���൵ 
public:
    UFUNCTION(BlueprintCallable)
    void SetTransparent(bool Transparent);
};
