// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaseThief.h"

#include "PatrolAgentBeliefs.h"

UChaseThief::UChaseThief()
{
	Priority = 2;
	GoalState.Add("ThiefVisible", true); 
}

bool UChaseThief::IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const
{
	if (const UPatrolAgentBeliefs* PatrolBeliefs = Cast<const UPatrolAgentBeliefs>(&Beliefs))
	{
		const FVector PatrolAgentPosition = WorldState.VectorStates["ThiefAgentLocation"];
		const FVector SightingOfThief = PatrolBeliefs->GetThiefCurrentLocation();
		return PatrolBeliefs->IsThiefVisible() == false|| FVector::Dist(PatrolAgentPosition, SightingOfThief) <= 100.0f ;
	}
	return false;
}

bool UChaseThief::IsGoalRelevant(const UWorldState& WorldState, UBeliefs& Beliefs) const
{
	return Beliefs.GetBeliefsState().Contains("ThiefVisible")
	&& Beliefs.GetBeliefsState()["ThiefVisible"]
	&& Beliefs.GetBeliefsStateVectors().Contains("ThiefVisible")
	&& Beliefs.GetBeliefsStateVectors()["ThiefVisible"] != FVector::Zero();
}
