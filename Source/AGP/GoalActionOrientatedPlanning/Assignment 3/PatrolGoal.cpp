// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolGoal.h"

#include "PatrolAgentBeliefs.h"

UPatrolGoal::UPatrolGoal()
{
	// low priority
	Priority = 1;
	GoalState.Add("AtWaypoint", true); 
}

bool UPatrolGoal::IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const
{
	/*
	if (Beliefs.GetBeliefsState().Contains("AtWaypoint") && Beliefs.GetBeliefsState()["AtWaypoint"])
	{
		// Optionally, check if agent has patrolled through all waypoints
		return true;  // Patrol goal is considered achieved when waypoints have been patrolled
	}

	return false;
	*/
	return false;
}

bool UPatrolGoal::IsGoalRelevant(const UWorldState& WorldState, UBeliefs& Beliefs) const
{
	return Beliefs.GetBeliefsState().Contains("ThiefVisible")
	&& !Beliefs.GetBeliefsState()["ThiefVisible"]
	&& Beliefs.GetBeliefsStateVectors().Contains("ThiefLastKnownPosition")
	&& Beliefs.GetBeliefsStateVectors()["ThiefLastKnownPosition"] == FVector::ZeroVector;
}
