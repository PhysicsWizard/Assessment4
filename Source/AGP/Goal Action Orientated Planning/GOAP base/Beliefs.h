// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldState.h"
#include "UObject/NoExportTypes.h"
#include "Beliefs.generated.h"

/**
 * 
 */
UCLASS()
class NAVMESH_TUTORIAL_API UBeliefs : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "GOAP")
	TMap<FString, bool> BeliefsState;
	TMap<FString, FVector> BeliefsStateVectors;

public:
	TMap<FString, bool> GetBeliefsState();
	TMap<FString, bool> GetBeliefsState() const;
	TMap<FString, FVector> GetBeliefsStateVectors();
	TMap<FString, FVector> GetBeliefsStateVectors() const;
	void UpdateBeliefsState(TMap<FString, bool> NewBeliefs);
	void UpdateBeliefsStateVectors(TMap<FString, FVector> NewBeliefs);
	bool BeliefsMatchesWorldState( const UWorldState& WorldState)const ;
	virtual UBeliefs* Clone();
};
