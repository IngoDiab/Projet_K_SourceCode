// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PK_Valid.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPK_Valid : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJET_K_API IPK_Valid
{
	GENERATED_BODY()
public:
	virtual bool IsValid()const PURE_VIRTUAL(, return false;);
};
