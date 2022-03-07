// Fill out your copyright notice in the Description page of Project Settings.


#include "StateBase.h"

#include "Components/StateMachine.h"

UDataStoreBase* UStateBase::GetDataStore() const
{
	return StateMachine ? StateMachine->GetDataStore() : nullptr;
}

void UStateBase::Initialize()
{
	OnInitialize();
}

void UStateBase::Entry(UDataStoreBase* GeneralData)
{
	OnEntry(GeneralData);
}

void UStateBase::Exit(UDataStoreBase* GeneralData)
{
	OnExit(GeneralData);
}
