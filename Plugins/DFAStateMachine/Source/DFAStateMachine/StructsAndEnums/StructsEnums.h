#pragma once

#include "Objects/StateBase.h"
#include "GameplayTagContainer.h"
#include "StructsEnums.generated.h"

USTRUCT(BlueprintType)
struct FStateInformation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	UStateBase* State = nullptr;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UStateBase> StateClassToCreate = nullptr;
	
	/* Transition to other states from current state. The tuple is < Transition ID, NextState >  */
	UPROPERTY(EditAnywhere, meta=(DisplayName="Transitions < Transition ID, NextState > "))
	TMap<FGameplayTag, FGameplayTag> Transitions;
};


