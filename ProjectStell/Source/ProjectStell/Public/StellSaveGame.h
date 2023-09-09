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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) //플레이중인 맵(레벨)의 이름
		FString PlayingLevelName=TEXT("None");
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) //플레이어의 위치 데이터
		FTransform PlayerLoctions ;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) //장착하고 있는 무기 종류(무기를바꿀때마다 저장하는게 아니라 세이브시에만 데이터를저장 할 것)
		FItemInfoStruct LeftWeapons;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) //장착하고 있는 무기 종류(무기를바꿀때마다 저장하는게 아니라 세이브시에만 데이터를저장 할 것)
		FItemInfoStruct RightWeapons;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) //해금한 무기들 데이터
		TMap<int32, FItemInfoStruct> UnLockWeapons;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) //캐릭터의 스텟 정보
		float CurHP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) //캐릭터의 스텟 정보
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
		FPlayerStruct PlayerStruct; //PlayerState에서 로드, 세이브 할 데이터
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		TMap<int32, FSectionStruct> Sections; //GameState에서 로드, 세이브 할 데이터

	UPROPERTY()
		int32 PlayTime;
	UPROPERTY()
		int32 DeadCount;
};
