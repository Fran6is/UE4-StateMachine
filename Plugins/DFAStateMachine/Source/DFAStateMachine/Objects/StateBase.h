// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStoreBase.h"
#include "UObject/Object.h"
#include "StateBase.generated.h"


UCLASS(Abstract, Blueprintable)
class UStateBase : public UObject
{
	GENERATED_BODY()

	friend class UStateMachine;

	UPROPERTY()
	class UStateMachine* StateMachine = nullptr;

	UPROPERTY()
	AActor* OwnerActor = nullptr;

	bool bIsActive = false;
	
protected:
	UFUNCTION(BlueprintCallable, Category=StateBase)
	UDataStoreBase* GetDataStore() const;

	UFUNCTION(BlueprintCallable, Category=StateBase)
	bool IsActive() const { return bIsActive;}

	UFUNCTION(BlueprintCallable, Category=StateBase)
	AActor* GetOwnerActor() const { return OwnerActor;}

	UFUNCTION(BlueprintCallable, Category=StateBase)
	UStateMachine* GetStateMachine() const { return StateMachine;}
	
	virtual void Initialize();
	
	UFUNCTION(BlueprintImplementableEvent, Category=StateBase)
	void OnInitialize();
	
	virtual void Entry(UDataStoreBase* GeneralData);

	UFUNCTION(BlueprintImplementableEvent, Category=StateBase)
	void OnEntry(UDataStoreBase* GeneralData);
	
	virtual void Exit(UDataStoreBase* GeneralData);

	UFUNCTION(BlueprintImplementableEvent, Category=StateBase)
	void OnExit(UDataStoreBase* GeneralData);
};
