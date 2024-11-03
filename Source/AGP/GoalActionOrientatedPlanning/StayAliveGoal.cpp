// Fill out your copyright notice in the Description page of Project Settings.
#include "AGP/GoalActionOrientatedPlanning/StayAliveGoal.h"

#include "EnemyAgent.h"
#include "EnemyAgentBeliefs.h"


class UEnemyAgentBeliefs;

UStayAliveGoal::UStayAliveGoal()
{
	GoalState.Add("InDangerOfDeath", false);
	Priority = 15;
}

bool UStayAliveGoal::IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const
{
	return Beliefs.GetBeliefsStateConst()["InDangerOfDeath"] == false;
}

bool UStayAliveGoal::IsGoalRelevant(const UWorldState& WorldState, UBeliefs& Beliefs) const
{
	return Beliefs.GetBeliefsState()["InDangerOfDeath"] == true;
}
