// Fill out your copyright notice in the Description page of Project Settings.


#include "Goal.h"

bool UGoal::IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const
{
	return Beliefs.BeliefsMatchesWorldState(WorldState);
}

void UGoal::UpdatePriority(int32 NewPriority)
{
	Priority = NewPriority;
}

bool UGoal::IsGoalRelevant(const UWorldState& WorldState, UBeliefs& Beliefs) const
{
	return true;
}


int32 UGoal::GetPriority()
{
	return Priority;
}
