// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine.h"


UStateMachine::UStateMachine()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

bool UStateMachine::Run()
{
	
	if(!bIsRunning)
	{
		const FStateInformation* StateInfoPtr = StatesDefinition.Find(StartState);

		//UE_LOG(LogTemp, Warning, TEXT("%s \t IsValid = %i"), *StartState.ToString(), StartState.IsValid() ? 1:0)
		
		if(StateInfoPtr && StateInfoPtr->State)
		{
			StateInfoPtr->State->bIsActive = true;
			StateInfoPtr->State->Entry(GeneralData);
			
			bIsRunning   = true;
			CurrentStateInfoPtr = StateInfoPtr;
			CurrentStateID = StartState;

			if(OnChangedStates.IsBound())
			{
				OnChangedStates.Broadcast(StartState, StartState);
			}

			UE_LOG(LogTemp, Warning, TEXT("UStateMachine::Run:: Machine is now running"))
			return true;
		}

		
		UE_LOG(LogTemp, Error, TEXT("UStateMachine::Run::LOG Machine Start state '%s' doesn't exist or no state object was created for it. "),
			*StartState.ToString())
			
		return false;
	}

	//UE_LOG(LogTemp, Warning, TEXT("UStateMachine::Run::LOG Machine already running"))
	
	return false;
}

bool UStateMachine::Transition(const FGameplayTag& TransitionSymbol)
{
	if(!bIsRunning)
	{
		UE_LOG(LogTemp, Error, TEXT("UStateMachine::Transition::LOG Machine isn't running yet \n"));
		return false;
	}
	// 'Map::Find' function will return a reference to the value (i.e pair.second ) from the map if it exists or null if it doesn't 
	if(const FGameplayTag* NextStateIDPtr = CurrentStateInfoPtr->Transitions.Find(TransitionSymbol) )
	{
		//TODO:
		//What to do if next state is same state? Loop or Ignore?
		//uncomment to ignore
		/*
		if(*NextStateIDPtr == CurrentStateID)
		{
			//UE_LOG(LogTemp, Error, TEXT("UStateMachine::Transition Same state "))
			return false;
		}
		*/
		const FStateInformation* NextStateInfoPtr = StatesDefinition.Find(*NextStateIDPtr);
		
		if( NextStateInfoPtr && NextStateInfoPtr->State )
		{
			CurrentStateInfoPtr->State->bIsActive = false;
			CurrentStateInfoPtr->State->Exit(GeneralData);

			NextStateInfoPtr->State->bIsActive = true;
			NextStateInfoPtr->State->Entry(GeneralData);
			
			if(OnChangedStates.IsBound())
			{
				OnChangedStates.Broadcast(CurrentStateID, *NextStateIDPtr);
			}
			
			CurrentStateID      = *NextStateIDPtr;
			CurrentStateInfoPtr =  NextStateInfoPtr;
			
			return true;
		}
		
		if(!NextStateInfoPtr)
		{
			UE_LOG(LogTemp, Error, TEXT("UStateMachine::Transition couldn't transition from '%s' to some next state because next state (map) item is null for some reason"),
			*CurrentStateID.ToString() )
		}
		else if (!NextStateInfoPtr->State)
		{
			UE_LOG(LogTemp, Error, TEXT("UStateMachine::Transition couldn't transition from current '%s' to next state '%s' because no object was created for next state. Either a null class was supplied in the editor or unreal couldn't create state object (for whatever reason)"),
			*CurrentStateID.ToString(),
			*NextStateIDPtr->ToString() )
		}
		
		return false;
	}
	
	UE_LOG(LogTemp, Error, TEXT("UStateMachine::Transition Passed in transition symbol '%s' doesn't exist for current state '%s'"),
		*TransitionSymbol.ToString(), *CurrentStateID.ToString())
	
	return false;
}

void UStateMachine::BeginPlay()
{
	Super::BeginPlay();
	
	for (auto& CurrentStateTuple : StatesDefinition)
	{
		if(CurrentStateTuple.Value.StateClassToCreate)
		{
			CurrentStateTuple.Value.State =	NewObject<UStateBase>(this, CurrentStateTuple.Value.StateClassToCreate);
			if(CurrentStateTuple.Value.State)
			{
				CurrentStateTuple.Value.State->StateMachine = this;
				CurrentStateTuple.Value.State->OwnerActor = this->GetOwner();
				CurrentStateTuple.Value.State->bIsActive = false;
				CurrentStateTuple.Value.State->Initialize();

			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("UStateMachine::LOG Unreal Couldn't create a state instance for state with id = '%s' and class name = '%s' for some reason "),
				  *CurrentStateTuple.Key.ToString(), *CurrentStateTuple.Value.StateClassToCreate->GetName()
				)

			}
			
			CurrentStateTuple.Value.StateClassToCreate = nullptr;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UStateMachine::LOG Couldn't create a state for id '%s' because class was empty"),
				*CurrentStateTuple.Key.ToString())
		}
		
	}

	if(ensureMsgf(DataStoreClass, TEXT("Data store class is Null. Null will be passed to every state on state's 'entry()' and 'exit()'")))
	{
		GeneralData = NewObject<UDataStoreBase>(this, DataStoreClass);
		
		ensureMsgf(GeneralData, TEXT("Couldn't instantiate a Data store object for some reason"));
	}
}

