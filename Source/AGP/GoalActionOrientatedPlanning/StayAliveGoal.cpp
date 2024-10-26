// Fill out your copyright notice in the Description page of Project Settings.
#include "AGP/GoalActionOrientatedPlanning/StayAliveGoal.h"

#include "EnemyAgentBeliefs.h"


class UEnemyAgentBeliefs;

UStayAliveGoal::UStayAliveGoal()
{
	Priority = 5;
}

bool UStayAliveGoal::IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const
{
	const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(&Beliefs);
	return EnemyBeliefs->GetBeliefsState()["InDangerOfDeath"] == false;
}

bool UStayAliveGoal::IsGoalRelevant(const UWorldState& WorldState, UBeliefs& Beliefs) const
{
	UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(&Beliefs);
	return EnemyBeliefs->GetBeliefsState()["InDangerOfDeath"] == true;
}
