// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructsAndEnums/StructsEnums.h"

#include "Components/ActorComponent.h"
#include "StateMachine.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStateChanged, FGameplayTag, PreviousState, FGameplayTag, CurrentState );

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class UStateMachine : public UActorComponent
{
	GENERATED_BODY()

public:
	UStateMachine();
    
    /*Will Run machine starting in 'StartState'*/
	UFUNCTION(BlueprintCallable, Category=StateMachine)
	bool Run();
	
	UFUNCTION(BlueprintCallable, Category=StateMachine)
    const FGameplayTag& GetCurrentStateTag() const{ return CurrentStateTag; };

	UFUNCTION(BlueprintCallable, Category=StateMachine)
	bool Transition(const FGameplayTag& TransitionSymbol);

	UFUNCTION(BlueprintCallable, Category=StateMachine)
	UDataStoreBase* GetDataStore(){ return GeneralData;}
	
	/* Called when machine successfully switches to a new state */
	UPROPERTY(BlueprintAssignable, Category=StateMachine)
	FOnStateChanged OnChangedStates;

protected:
	virtual void BeginPlay() override;

private:
	bool bIsRunning = false;
	
	const FStateInformation* CurrentStateInfoPtr = nullptr;

	FGameplayTag CurrentStateTag;
	
	//Pointer to Data to pass to active state on 'entry()' and 'exit()' function calls
	UPROPERTY()
	UDataStoreBase* GeneralData = nullptr;

	//Data class to instantiate and pass to active state on 'entry()' and 'exit()' function calls
	//Derive your own custom class from "DataStoreBase' 
	UPROPERTY(EditAnywhere, Category=Setup)
	TSubclassOf<UDataStoreBase> DataStoreClass = UDataStoreBase::StaticClass();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup , meta=(AllowPrivateAccess=true))
	FGameplayTag StartState;

	/* 
	 * 'StatesDefinition' is a map/dictionary of states and their unique IDs (as Gameplay tags). The tuple is < State ID, State Definition >
	 * You Create a state by providing: (1) a Tag/ID, (2) state class (to create for state), and (3) (maybe) add transitions to other states.
	 * 
	 * State's 'Transitions' property will contain your connection(s) to other states. The tuple is < Transition ID, Next State >
	 * When creating a transition; First provide a transition ID (key) and the next state to go to (value). Make sure the next state already exist in 'StatesDefinition' or its going to be added
	 *
	 * */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setup, meta=(DisplayName="States Definition < State ID, State Information > ", AllowPrivateAccess=true))
	TMap<FGameplayTag , FStateInformation> StatesDefinition;
};
