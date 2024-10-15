// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Beliefs.h"
#include "WorldState.h"
#include "UObject/NoExportTypes.h"
#include "Action.generated.h"


/**
 * 
 */
UCLASS()
class NAVMESH_TUTORIAL_API UAction : public UObject
{
	GENERATED_BODY()

protected:
	//variables
	UPROPERTY(EditAnywhere, Category="GOAP")
	TMap<FString,bool> Preconditions;
	TMap<FString,FVector> PreconditionsVectors;

	UPROPERTY(EditAnywhere, Category="GOAP")
	TMap<FString,bool> Effects;
	TMap<FString,FVector> EffectVectors;

	UPROPERTY(EditAnywhere, Category="GOAP")
	float cost;

	UPROPERTY()
	ACharacter* Character;  
	
public:
	FTimerHandle TimerHandle;
	//functions
	TMap<FString,bool> GetPreconditions();
	TMap<FString,FVector> GetPreconditionsVectors();
	TMap<FString,bool> GetEffects();
	TMap<FString,FVector> GetEffectsVectors();
	float Getcost();
	virtual bool IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs);
	virtual void ApplyEffects (UWorldState& WorldState);
	virtual bool IsActionComplete() const;
	virtual void PerformAction();
};
