// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"

#include "Petten/SpawnAble.h"

#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemType :uint8
{
	None = 0, Weapon, Consumer, ETC
};
USTRUCT(Atomic, BlueprintType)
struct FItemInfoStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemInfo)
		int32 ID = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemInfo)
		TSubclassOf<class AActor> ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemInfo)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemInfo)
		EItemType Type = EItemType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemInfo)
		int32 Quantity = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemInfo)
		UTexture2D* Icon = nullptr;

};

UCLASS()
class PROJECTSTELL_API AItem : public AActor, public ISpawnAble
{
	GENERATED_BODY()
public:	
	AItem();
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, Meta = (AllowPrivateAccess = true))
		class USkeletalMeshComponent* mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item, Meta = (AllowPrivateAccess = true))
		FItemInfoStruct info;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = box, Meta = (AllowPrivateAccess = true))
		UBoxComponent* box;
	UPROPERTY(VisibleAnywhere)
		URotatingMovementComponent* movement;
public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
		float reSpawnTime;
		FItemInfoStruct GetItemInfo();
		void Acquiring_Item();
		FTimerHandle ItemReSpawnTimmerHandle;
private:
	void ReSpawnItemTimmer();
	float CurSpawnTime;
		/*�÷��̾ �����ۿ� �����ؼ� ���� ��ư�� ������ ȣ��� 
		* ������ ������� �ϱ� �Լ� �� �ڵ����� �ٽ� ��Ÿ���� �� �Լ� ��������Ʈ�� ���ε��ϱ� ����ε��ϱ� 
		* 
		*/

};
