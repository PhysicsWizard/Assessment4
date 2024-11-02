// Fill out your copyright notice in the Description page of Project Settings.
#include "AGP/GoalActionOrientatedPlanning/StayAliveGoal.h"

#include "EnemyAgent.h"
#include "EnemyAgentBeliefs.h"


class UEnemyAgentBeliefs;

UStayAliveGoal::UStayAliveGoal()
{
	GoalState.Add("Dying", false);
	Priority = 5;
}

bool UStayAliveGoal::IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(EnemyAgent->GetBeliefs());
	return EnemyBeliefs->GetBeliefsState()["InDangerOfDeath"] == false;
}

bool UStayAliveGoal::IsGoalRelevant(const UWorldState& WorldState, UBeliefs& Beliefs) const
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(EnemyAgent->GetBeliefs());
	return EnemyBeliefs->GetBeliefsState()["InDangerOfDeath"] == true;
}
