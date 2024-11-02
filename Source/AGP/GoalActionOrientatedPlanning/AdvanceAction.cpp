// Fill out your copyright notice in the Description page of Project Settings.
#include "AGP/GoalActionOrientatedPlanning/AdvanceAction.h"

#include "EnemyAgent.h"
#include "EnemyAgentBeliefs.h"
#include "AGP/Characters/EnemyCharacter.h"

UAdvanceAction::UAdvanceAction()
{
	cost = 1.0f;
	Effects.Add("WithinRange", true);
	Effects.Add("AttackingTarget", true); 
}

bool UAdvanceAction::IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter()); 
	//check if a target is directly spotted
	const bool bTargetSpotted = EnemyAgent->GetBeliefs()->GetBeliefsState()["TargetSpotted"];
	const bool bWithinRange = EnemyAgent->GetBeliefs()->GetBeliefsState()["WithinRange"];

	return bTargetSpotted && !bWithinRange;
}

void UAdvanceAction::PerformAction()
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOuter()->GetOuter());
	EnemyCharacter->TickAdanceToTarget();
	
}

bool UAdvanceAction::IsActionComplete() const
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter()); 
	//check if a target is directly spotted
	const bool bTargetSpotted = EnemyAgent->GetBeliefs()->GetBeliefsState()["TargetSpotted"];
	const bool bWithinRange = EnemyAgent->GetBeliefs()->GetBeliefsState()["WithinRange"];

	return !bTargetSpotted || bWithinRange;
}

void UAdvanceAction::ApplyEffects(UWorldState& WorldState)
{
	Super::ApplyEffects(WorldState);
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter()); 
	EnemyAgent->GetBeliefs()->GetBeliefsState()["WithinRange"] = true; 
}
