// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "Goal.h"
#include "UObject/NoExportTypes.h"
#include "Planner.generated.h"

// Forward declaration
class UAgent;
class UWorldState;
class UBeliefs;
/**
 * 
 */
UCLASS()
class AGP_API UPlanner : public UObject
{
	GENERATED_BODY()
protected:
	//variables
	UPROPERTY(EditAnywhere, Category="GOAP")
	TArray<UAction*> ActionPool;
	
public:
	//functions
	TArray<UAction*> CreatePlan(UAgent* Agent, UGoal* Goal, UWorldState& WorldState , UBeliefs& Beliefs);
	TArray<UAction*> FindBestPlan(UAgent* Agent, UWorldState& WorldState, UBeliefs& Beliefs);
	
};
