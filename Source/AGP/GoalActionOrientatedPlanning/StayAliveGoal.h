// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAP base/Goal.h"
#include "StayAliveGoal.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API UStayAliveGoal : public UGoal
{
	GENERATED_BODY()
	UStayAliveGoal();
	virtual bool IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const override;
	virtual bool IsGoalRelevant(const UWorldState& WorldState,  UBeliefs& Beliefs) const;
	
};
