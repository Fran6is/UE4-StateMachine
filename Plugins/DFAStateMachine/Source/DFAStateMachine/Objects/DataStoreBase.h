// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DataStoreBase.generated.h"

/**
 * Base data object class to be instantiated and pass to an active machine state
 */
UCLASS(Blueprintable)
class UDataStoreBase : public UObject
{
	GENERATED_BODY()
};
