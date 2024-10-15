// Fill out your copyright notice in the Description page of Project Settings.


#include "StealItemGoal.h"

UStealItemGoal::UStealItemGoal()
{
	Priority = 2;
	GoalState.Add("GoldenTaken", true);
}

bool UStealItemGoal::IsGoalAchieved(const UWorldState& WorldState, const UBeliefs& Beliefs) const
{
	return WorldState.GetWorldState()["GoldTaken"];
}

bool UStealItemGoal::IsGoalRelevant(const UWorldState& WorldState, UBeliefs& Beliefs) const
{
	const bool bDosentHaveGold = !WorldState.GetWorldState()["GoldTaken"];
	const bool bIsHidden = WorldState.GetWorldState()["ThiefVisible"] == false;
	return bDosentHaveGold && bIsHidden ;
}
