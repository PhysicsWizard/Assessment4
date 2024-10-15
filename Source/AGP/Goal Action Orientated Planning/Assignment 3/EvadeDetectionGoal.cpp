// Fill out your copyright notice in the Description page of Project Settings.


#include "EvadeDetectionGoal.h"

UEvadeDetectionGoal::UEvadeDetectionGoal()
{
	Priority = 5;
	GoalState.Add("ThiefVisible", false);
}

bool UEvadeDetectionGoal::IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const
{
	return WorldState.GetWorldState()["ThiefVisible"] == false;
}

bool UEvadeDetectionGoal::IsGoalRelevant(const UWorldState& WorldState, UBeliefs& Beliefs) const
{
	return WorldState.GetWorldState()["ThiefVisible"] == true;
}
