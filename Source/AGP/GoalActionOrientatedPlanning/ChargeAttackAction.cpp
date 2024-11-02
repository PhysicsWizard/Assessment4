// Fill out your copyright notice in the Description page of Project Settings.
#include "AGP/GoalActionOrientatedPlanning/ChargeAttackAction.h"

#include "EnemyAgent.h"
#include "AGP/Characters/EnemyCharacter.h"

UChargeAttackAction::UChargeAttackAction()
{
	cost = 7.0f;
	Preconditions.Add("TargetSpotted", true);
	Preconditions.Add("AttackingTarget", false);
	Preconditions.Add("WithinRange", true);
	Preconditions.Add("HasFullHealth", true);
	Effects.Add("AttackingTarget", true);
}

bool UChargeAttackAction::IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter()); 
	const bool bTargetSpotted = EnemyAgent->GetBeliefs()->GetBeliefsState()["TargetSpotted"];
	const bool bNotWithinFiringRange = !EnemyAgent->GetBeliefs()->GetBeliefsState()["WithinRange"];
	
	return bTargetSpotted && bNotWithinFiringRange; 
}

void UChargeAttackAction::PerformAction()
{
	if(AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOuter()->GetOuter()))
	{ 
		EnemyCharacter->TickEngage();
	}
}

bool UChargeAttackAction::IsActionComplete() const
{
	 
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const bool bNoTarget = EnemyAgent->GetBeliefs()->GetBeliefsState()["TargetSpotted"] == false;
	const bool bWithinStationFireRange = EnemyAgent->GetBeliefs()->GetBeliefsState()["WithinRange"] == true;
	const bool bInDangerOfDeath = EnemyAgent->GetBeliefs()->GetBeliefsState()["InDangerOfDeath"] == true;
	
	if (bNoTarget || bWithinStationFireRange || bInDangerOfDeath)
	{
		// Reset AttackingTarget to false
		EnemyAgent->GetBeliefs()->GetBeliefsState()["AttackingTarget"] = false;
		return true;
	}
	return false;
}

void UChargeAttackAction::ApplyEffects(UWorldState& WorldState)
{
	Super::ApplyEffects(WorldState);
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(EnemyAgent->GetBeliefs());
	EnemyBeliefs->GetBeliefsState()["AttackingTarget"] = true;
}
