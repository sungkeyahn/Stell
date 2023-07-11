// Fill out your copyright notice in the Description page of Project Settings.


#include "Stell.h"

UStell::UStell()
{
	//���ӿ��� ���� ��� ���������̺� ���� �ּ��� �����ϴ� �κ� 
	FString path = TEXT("DataTable'/Game/1_DataTable/BaseStatData.BaseStatData'");
	static ConstructorHelpers::FObjectFinder<UDataTable>d(*path);
	if (d.Succeeded()) StatDataTable = d.Object;
	if (StatDataTable->GetRowMap().Num() < 0) return;

	path = TEXT("DataTable'/Game/1_DataTable/PlayerStatDataByLevel.PlayerStatDataByLevel'");
	static ConstructorHelpers::FObjectFinder<UDataTable>dd(*path);
	if (dd.Succeeded()) StatDataTable2 = dd.Object;
	if (StatDataTable2->GetRowMap().Num() < 0) return;
}
void UStell::Init()
{
	Super::Init();
}
FBaseStatData* UStell::GetDataTableRow(int32 Level)
{
	return StatDataTable->FindRow<FBaseStatData>(*FString::FromInt(Level), TEXT(""));
}
FPlayerStatDataByLevel* UStell::GetPlayerStatDataTableRow(int32 Level)
{
	return StatDataTable2->FindRow<FPlayerStatDataByLevel>(*FString::FromInt(Level), TEXT(""));
}
