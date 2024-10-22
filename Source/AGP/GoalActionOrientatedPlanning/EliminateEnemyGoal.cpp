// Fill out your copyright notice in the Description page of Project Settings.
#include "AGP/GoalActionOrientatedPlanning/EliminateEnemyGoal.h"

#include "EnemyAgentBeliefs.h"


UEliminateEnemyGoal::UEliminateEnemyGoal()
{
    Priority = 3;
    GoalState.Add("AttackingTarget", true);
}

bool UEliminateEnemyGoal::IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const
{
    const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(&Beliefs);
    const bool bTargetNoLongerVisible = !EnemyBeliefs->GetTarget();
    const bool bOutOfRange = EnemyBeliefs->GetBeliefsState()["WithinRange"];
    return bTargetNoLongerVisible || bOutOfRange;
}

bool UEliminateEnemyGoal::IsGoalRelevant(const UWorldState& WorldState, UBeliefs& Beliefs) const
{
    UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(&Beliefs);
    const bool bHasTarget = EnemyBeliefs->GetTarget();
    const bool bWithinRange_ToFire = EnemyBeliefs->GetBeliefsState()["WithinRange"];
    return  bHasTarget && bWithinRange_ToFire;
}


