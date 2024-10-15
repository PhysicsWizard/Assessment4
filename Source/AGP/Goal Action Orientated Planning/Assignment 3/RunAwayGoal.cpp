// Fill out your copyright notice in the Description page of Project Settings.


#include "RunAwayGoal.h"

#include "PatrolAgentBeliefs.h"
#include "ThiefAgent.h"

class UPatrolAgentBeliefs;

URunAwayGoal::URunAwayGoal()
{
	Priority = 2;
	GoalState.Add("Escaped", true);
}

bool URunAwayGoal::IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const
{
	const FVector PatrolAgentPosition = WorldState.VectorStates["ThiefAgentLocation"];
	const FVector ExitVectorLocation(1490.0f, -250.0f, 460.0f);
	
	return  FVector::Dist(PatrolAgentPosition, ExitVectorLocation) <= 100.0f;
}

bool URunAwayGoal::IsGoalRelevant(const UWorldState& WorldState, UBeliefs& Beliefs) const
{
	return WorldState.GetWorldState().Contains("GoldTaken")
	&& WorldState.GetWorldState()["GoldTaken"] == true
	&& WorldState.GetWorldState().Contains("Escaped")
	&& WorldState.GetWorldState()["Escaped"] == false;
}
