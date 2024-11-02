// Fill out your copyright notice in the Description page of Project Settings.
#include "AGP/GoalActionOrientatedPlanning/AttackAction.h"

#include "EnemyAgent.h"
#include "EnemyAgentBeliefs.h"
#include "AGP/Characters/EnemyCharacter.h"

UAttackAction::UAttackAction()
{
	cost = 5.0f;
	Effects.Add("AttackingTarget", true);
}

bool UAttackAction::IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const bool bTargetSpotted = EnemyAgent->GetBeliefs()->GetBeliefsState()["TargetSpotted"];
	const bool bWithinFiringRange = EnemyAgent->GetBeliefs()->GetBeliefsState()["WithinRange"];
	return bTargetSpotted && bWithinFiringRange;
}

void UAttackAction::PerformAction()
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOuter()->GetOuter());
	EnemyCharacter->TickEngageStationary();
}

bool UAttackAction::IsActionComplete() const
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter()); 
	const bool bNoTarget = !EnemyAgent->GetBeliefs()->GetBeliefsState()["TargetSpotted"];
	const bool bOutOfRange = !EnemyAgent->GetBeliefs()->GetBeliefsState()["WithinRange"];
	const bool bInDangerOfDeath = EnemyAgent->GetBeliefs()->GetBeliefsState()["InDangerOfDeath"];
	
	if (bNoTarget || bOutOfRange|| bInDangerOfDeath)
	{
		// Reset AttackingTarget to false
		EnemyAgent->GetBeliefs()->GetBeliefsState()["AttackingTarget"] = false;
		return true;
	}
	return false;
}

void UAttackAction::ApplyEffects(UWorldState& WorldState)
{
	Super::ApplyEffects(WorldState);
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter()); 
	EnemyAgent->GetBeliefs()->GetBeliefsState()["AttackingTarget"] = true;
}
