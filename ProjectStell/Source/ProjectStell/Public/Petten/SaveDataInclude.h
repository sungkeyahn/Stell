// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "UObject/Interface.h"
#include "SaveDataInclude.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveDataInclude : public UInterface
{
	GENERATED_BODY()
};

class PROJECTSTELL_API ISaveDataInclude
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void DataSaveFun()=0;
	virtual void DataLoadFun() = 0;

};
