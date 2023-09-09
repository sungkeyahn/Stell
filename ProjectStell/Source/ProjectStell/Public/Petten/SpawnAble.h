// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "UObject/Interface.h"
#include "SpawnAble.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class USpawnAble : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTSTELL_API ISpawnAble
{
	GENERATED_BODY()
private:
	int32 ObjectNumber;
public:
	int32 GetObjectNumber() { return ObjectNumber; }
	void SetObjectNumber(int32 num) { ObjectNumber = num; }
	void DeleteObject(class ASection* Section);
	/*
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		AActor* Clone(TSubclassOf<class AActor> info);
	*/

};
