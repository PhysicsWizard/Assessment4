// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"     
#include "AGP/Goal Action Orientated Planning/GOAP base/Goal.h"
#include "PatrolGoal.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API UPatrolGoal : public UGoal
{
	GENERATED_BODY()

public:
	UPatrolGoal();

	// Override the IsGoalAchieved function
	virtual bool IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const override;
	virtual bool IsGoalRelevant(const UWorldState& WorldState, UBeliefs& Beliefs) const;
};
