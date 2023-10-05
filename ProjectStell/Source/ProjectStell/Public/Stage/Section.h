// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/Actor.h"

#include "StellSaveGame.h"
#include "Petten/SaveDataInclude.h"

#include "Section.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSectionClearDelegate);

UCLASS()
class PROJECTSTELL_API ASection : public AActor, public ISaveDataInclude
{
	GENERATED_BODY()
	
public:	
	ASection();  
protected:
	virtual void BeginPlay() override;
public:
	virtual void PostInitializeComponents()override;
	/*대강 맵에 배치하는 하나의 맵 매니저 개념으로 사용할 예정
	* 스폰 기능
	스포너 기능을 통하여 지정한 위치에 다양한 몬스터,오브젝트들을 생성시킬수 있음
	따라서 섹션에 배치될 몬스터의 정보를 편집할수 있도록 변수로 가지고 있어야함
	또한 스폰되는 오브젝트가 게임세이브에 영향을 받음
	* 이동 제한/해제 기능
	지정된 투명벽(액터)을 게임스코어에 따라 생성(스폰기능으로 생성할 예정)/삭제 하는 기능-->이건 투명벽 액터가 해야하는 일
	포탈을 통하여 다음 레벨으로 캐릭터가 이동하는것에 대한 정보를 처리
	맵 오브젝트들에 대한 상호작용 호출은 게임인스턴스에서 
	*배치된 오브젝트들에 대한 컨트롤 기능?
	* 게임 진행도 기록/저장 기능
	죽은 몬스터/살아있는 몬스터들에 대한 정보들 + 
	플레이어가 스테이지(맵)에서 어느섹션 까지 통과했는지에 대한 정보들을 기록하는 변수 필요
	그리고 게임이 종료 혹은 진행중 조건을 만족하는 상황에 해당 상황에 대한 데이터를 저장 하는 기능	
	*/
private:
	//아래3개의 변수는 에디터 환경에서 설정하기 위해 만들어둔 변수들
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Section, meta = (AllowPrivateAccess = "true"))
		int32 SectionNum;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Section, meta = (AllowPrivateAccess = "true"))
		TArray<FSpawnStruct> DefaultSpawnList;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Section, meta = (AllowPrivateAccess = "true"))
		int32 SectionClearScore = 0;
public:
	//데이터 로딩을 런타임동안 저장할 변수
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Section, meta = (AllowPrivateAccess = "true"))
		FSectionStruct CurSectioninfo;

protected:
	virtual void SectionClearConditionCheck();

public:
	FSectionStruct& GetSectionInfo() {return CurSectioninfo;}
	FOnSectionClearDelegate OnSectionClear;
	void AddSectionClearScore();

	void Spawn(int32 index);
	void DefaultSpawn(int32 index);

	virtual void DataSaveFun() override;
	virtual void DataLoadFun() override;

};
