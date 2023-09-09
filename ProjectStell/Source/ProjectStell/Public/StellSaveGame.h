// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"

#include "GameFramework/SaveGame.h"
#include "Object/Item.h"

#include "StellSaveGame.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FSpawnStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESpawnObectType ObectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform SpawnTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AActor> Prototype;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDestroy;
};

USTRUCT(Atomic, BlueprintType)
struct FPlayerStruct
{
	GENERATED_BODY()
public:
	FPlayerStruct() {};
	FPlayerStruct(FString pn, FTransform loc, FItemInfoStruct left, FItemInfoStruct right, TMap<int32, FItemInfoStruct> unlock, float hp, float sp)
	:PlayingLevelName(pn), PlayerLoctions(loc), LeftWeapons(left), RightWeapons(right), UnLockWeapons(unlock), CurHP(hp), CurSP(sp)
	{};
	FPlayerStruct(FString pn, FTransform loc, TMap<int32, FItemInfoStruct> unlock, float hp, float sp)
		:PlayingLevelName(pn), PlayerLoctions(loc), UnLockWeapons(unlock), CurHP(hp), CurSP(sp)
	{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) //�÷������� ��(����)�� �̸�
		FString PlayingLevelName=TEXT("None");
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) //�÷��̾��� ��ġ ������
		FTransform PlayerLoctions ;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) //�����ϰ� �ִ� ���� ����(���⸦�ٲܶ����� �����ϴ°� �ƴ϶� ���̺�ÿ��� �����͸����� �� ��)
		FItemInfoStruct LeftWeapons;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) //�����ϰ� �ִ� ���� ����(���⸦�ٲܶ����� �����ϴ°� �ƴ϶� ���̺�ÿ��� �����͸����� �� ��)
		FItemInfoStruct RightWeapons;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) //�ر��� ����� ������
		TMap<int32, FItemInfoStruct> UnLockWeapons;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) //ĳ������ ���� ����
		float CurHP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) //ĳ������ ���� ����
		float CurSP;
};

USTRUCT(Atomic, BlueprintType)
struct FSectionStruct
{
	GENERATED_BODY()
public:
	UPROPERTY() 
		int32 SectionClearScore;
	UPROPERTY()
		bool IsSectionClear;
	UPROPERTY()
		TArray<FSpawnStruct> SpawnList;
};


UCLASS()
class PROJECTSTELL_API UStellSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UStellSaveGame();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		FString SaveSlotName;	// Save game data file name
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		int32 SaveIndex;		// Save game data file index

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		FPlayerStruct PlayerStruct; //PlayerState���� �ε�, ���̺� �� ������
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		TMap<int32, FSectionStruct> Sections; //GameState���� �ε�, ���̺� �� ������

	UPROPERTY()
		int32 PlayTime;
	UPROPERTY()
		int32 DeadCount;
};
