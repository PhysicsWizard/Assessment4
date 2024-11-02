// Fill out your copyright notice in the Description page of Project Settings.
#include "AGP/GoalActionOrientatedPlanning/EliminateEnemyGoal.h"

#include "EnemyAgent.h"
#include "EnemyAgentBeliefs.h"


UEliminateEnemyGoal::UEliminateEnemyGoal()
{
    Priority = 3;
}

bool UEliminateEnemyGoal::IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const
{
    const bool bPlayerOneDead = WorldState.GetWorldState()["Player_One_Dead"];
    const bool bPlayerTwoDead = WorldState.GetWorldState()["Player_Two_Dead"];
    return  bPlayerOneDead && bPlayerTwoDead;
}

bool UEliminateEnemyGoal::IsGoalRelevant(const UWorldState& WorldState, UBeliefs& Beliefs) const
{
    UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
    const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(EnemyAgent->GetBeliefs());
    const bool bNotAttackingTarget = !EnemyBeliefs->GetBeliefsState()["AttackingTarget"];
    const bool bNotInDangerOfDeath = EnemyBeliefs->GetBeliefsState()["InDangerOfDeath"];
    const bool bPlayerOneAlive = !WorldState.GetWorldState()["Player_One_Dead"];
    const bool bPlayerTwoAlive = !WorldState.GetWorldState()["Player_Two_Dead"];
    return bNotInDangerOfDeath
            && bNotAttackingTarget
            && bPlayerOneAlive
            && bPlayerTwoAlive;
}


