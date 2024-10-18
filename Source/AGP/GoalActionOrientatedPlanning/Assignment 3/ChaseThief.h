// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"  
#include "AGP/GoalActionOrientatedPlanning/GOAP base/Goal.h"
#include "ChaseThief.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API UChaseThief : public UGoal
{
	GENERATED_BODY()

public:
	UChaseThief();
	virtual bool IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const override;
	virtual bool IsGoalRelevant(const UWorldState& WorldState, UBeliefs& Beliefs) const;
};
