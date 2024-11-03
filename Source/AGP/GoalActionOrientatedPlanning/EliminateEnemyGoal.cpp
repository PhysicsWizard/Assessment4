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
    const bool bPlayerOneDead = WorldState.GetWorldState()["Player_One_Dead"];
    const bool bPlayerTwoDead = WorldState.GetWorldState()["Player_Two_Dead"];
    const bool bInDangerOfDeath = Beliefs.GetBeliefsStateConst()["InDangerOfDeath"];
    WorldState.GetWorldState()["AllEnemiesEliminated"] = bPlayerOneDead && bPlayerTwoDead;
    return WorldState.GetWorldState()["AllEnemiesEliminated"] && !bInDangerOfDeath;
}

bool UEliminateEnemyGoal::IsGoalRelevant(const UWorldState& WorldState, UBeliefs& Beliefs) const
{
    const bool bInDangerOfDeath = Beliefs.GetBeliefsState()["InDangerOfDeath"];
    return !WorldState.GetWorldState()["AllEnemiesEliminated"] || bInDangerOfDeath;
}


