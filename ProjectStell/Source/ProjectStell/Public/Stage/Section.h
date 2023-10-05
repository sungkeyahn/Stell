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
	/*�밭 �ʿ� ��ġ�ϴ� �ϳ��� �� �Ŵ��� �������� ����� ����
	* ���� ���
	������ ����� ���Ͽ� ������ ��ġ�� �پ��� ����,������Ʈ���� ������ų�� ����
	���� ���ǿ� ��ġ�� ������ ������ �����Ҽ� �ֵ��� ������ ������ �־����
	���� �����Ǵ� ������Ʈ�� ���Ӽ��̺꿡 ������ ����
	* �̵� ����/���� ���
	������ ����(����)�� ���ӽ��ھ ���� ����(����������� ������ ����)/���� �ϴ� ���-->�̰� ���� ���Ͱ� �ؾ��ϴ� ��
	��Ż�� ���Ͽ� ���� �������� ĳ���Ͱ� �̵��ϴ°Ϳ� ���� ������ ó��
	�� ������Ʈ�鿡 ���� ��ȣ�ۿ� ȣ���� �����ν��Ͻ����� 
	*��ġ�� ������Ʈ�鿡 ���� ��Ʈ�� ���?
	* ���� ���൵ ���/���� ���
	���� ����/����ִ� ���͵鿡 ���� ������ + 
	�÷��̾ ��������(��)���� ������� ���� ����ߴ����� ���� �������� ����ϴ� ���� �ʿ�
	�׸��� ������ ���� Ȥ�� ������ ������ �����ϴ� ��Ȳ�� �ش� ��Ȳ�� ���� �����͸� ���� �ϴ� ���	
	*/
private:
	//�Ʒ�3���� ������ ������ ȯ�濡�� �����ϱ� ���� ������ ������
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Section, meta = (AllowPrivateAccess = "true"))
		int32 SectionNum;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Section, meta = (AllowPrivateAccess = "true"))
		TArray<FSpawnStruct> DefaultSpawnList;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Section, meta = (AllowPrivateAccess = "true"))
		int32 SectionClearScore = 0;
public:
	//������ �ε��� ��Ÿ�ӵ��� ������ ����
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
