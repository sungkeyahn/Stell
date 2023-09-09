// Fill out your copyright notice in the Description page of Project Settings.


#include "Petten/SpawnAble.h"

#include "Stage/Section.h"
// Add default functionality here for any ISpawnAble functions that are not pure virtual.

/*
void ISpawnAble::Execute_Clone(UObject* actor)
{
}
*/

void ISpawnAble::DeleteObject(ASection* Section)
{
	if (Section)
		if (Section->CurSectioninfo.SpawnList.IsValidIndex(ObjectNumber))
			Section->CurSectioninfo.SpawnList[ObjectNumber].IsDestroy = true;
}
