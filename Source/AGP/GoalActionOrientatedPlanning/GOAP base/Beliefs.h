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
class AGP_API UBeliefs : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "GOAP")
	TMap<FString, bool> BeliefsState;
	TMap<FString, FVector> BeliefsStateVectors;

public:
	// Non-const versions return a reference
	TMap<FString, bool>& GetBeliefsState();
	TMap<FString, FVector>& GetBeliefsStateVectors();
	// Const versions return a const reference
	const TMap<FString, bool>& GetBeliefsStateConst() const;
	const TMap<FString, FVector>& GetBeliefsStateVectorsConst() const;
	void UpdateBeliefsState(TMap<FString, bool> NewBeliefs);
	void UpdateBeliefsStateVectors(TMap<FString, FVector> NewBeliefs);
	void UpdateBeliefsState(const TPair<FString, bool>& NewBeliefs);
	void UpdateBeliefsStateVectors(const TPair<FString, FVector>& NewBeliefs);
	bool BeliefsMatchesWorldState( const UWorldState& WorldState)const ;
	virtual UBeliefs* Clone();
};
