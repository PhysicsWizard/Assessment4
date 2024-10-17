// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGP/Goal Action Orientated Planning/GOAP base/Goal.h"
#include "StealItemGoal.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API UStealItemGoal : public UGoal
{
	GENERATED_BODY()

public:
	UStealItemGoal();
	virtual bool IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const override;
	virtual bool IsGoalRelevant(const UWorldState& WorldState, UBeliefs& Beliefs) const;
	
};
