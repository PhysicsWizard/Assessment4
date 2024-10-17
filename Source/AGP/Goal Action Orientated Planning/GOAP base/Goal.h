// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Beliefs.h"
#include "WorldState.h"
#include "UObject/NoExportTypes.h"
#include "Goal.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API UGoal : public UObject
{
	GENERATED_BODY()

protected:
	//variableds
	UPROPERTY(EditAnywhere, Category="GOAP")
	TMap<FString,bool> GoalState;

	UPROPERTY(EditAnywhere, Category="GOAP")
	TMap<FString,FVector> GoalStateVectors;
	
	UPROPERTY(EditAnywhere, Category="GOAP")
	int32 Priority;
	
public:
	//functions
	virtual bool IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const;
	void UpdatePriority(int32 NewPriority);
	virtual bool IsGoalRelevant(const UWorldState& WorldState, UBeliefs& Beliefs) const;
	int32 GetPriority();
	
};
