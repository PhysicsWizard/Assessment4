// Fill out your copyright notice in the Description page of Project Settings.
#include "AGP/GoalActionOrientatedPlanning/EliminateEnemyGoal.h"

#include "EnemyAgent.h"
#include "EnemyAgentBeliefs.h"


UEliminateEnemyGoal::UEliminateEnemyGoal()
{
    GoalState.Add("AllEnemiesEliminated", true);
    Priority = 10; // Higher priority than stay alive
}

bool UEliminateEnemyGoal::IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const
{ 
    const bool bAllEnemiesEliminated = WorldState.GetWorldState()["AllEnemiesEliminated"];
    return bAllEnemiesEliminated;
}

bool UEliminateEnemyGoal::IsGoalRelevant(const UWorldState& WorldState, UBeliefs& Beliefs) const
{
    const bool bAllEnemiesEliminated = WorldState.GetWorldState()["AllEnemiesEliminated"];
    return !bAllEnemiesEliminated;
}


