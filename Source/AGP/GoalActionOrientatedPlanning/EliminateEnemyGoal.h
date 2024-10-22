// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAP base/Goal.h"
#include "EliminateEnemyGoal.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API UEliminateEnemyGoal : public UGoal
{
	GENERATED_BODY()
	UEliminateEnemyGoal();
	virtual bool IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const override;
	virtual bool IsGoalRelevant(const UWorldState& WorldState,  UBeliefs& Beliefs) const;
};
