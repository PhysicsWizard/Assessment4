// Fill out your copyright notice in the Description page of Project Settings.


#include "InvestigateGoal.h"

#include "PatrolAgentBeliefs.h"

UInvestigateGoal::UInvestigateGoal()
{
	GoalState.Add("ThiefVisible", true);
	Priority = 3; 
}

bool UInvestigateGoal::IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const
{
	if (const UPatrolAgentBeliefs* PatrolBeliefs = Cast<const UPatrolAgentBeliefs>(&Beliefs))
	{
		const FVector PatrolAgentPosition = WorldState.GetWorldVectorState()["PatrolAgentPosition"];
		const FVector LastKnown_SightingOfThief = PatrolBeliefs->GetThiefsLastKnownLocation();
		return PatrolBeliefs->IsThiefVisible() || FVector::Dist(PatrolAgentPosition, LastKnown_SightingOfThief) <= 50.0f;
	}
	return false;
}

bool UInvestigateGoal::IsGoalRelevant(const UWorldState& WorldState,  UBeliefs& Beliefs) const
{
	return Beliefs.GetBeliefsState().Contains("ThiefVisible")
	&& !Beliefs.GetBeliefsState()["ThiefVisible"]
	&& Beliefs.GetBeliefsStateVectors().Contains("ThiefLastKnownPosition")
	&& Beliefs.GetBeliefsStateVectors()["ThiefLastKnownPosition"] != FVector::ZeroVector;
				
}
