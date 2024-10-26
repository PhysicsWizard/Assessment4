// Fill out your copyright notice in the Description page of Project Settings.
#include "AGP/GoalActionOrientatedPlanning/ChargeAttackAction.h"

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
	const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(&Beliefs);
	const bool bTargetSpotted = EnemyBeliefs->GetBeliefsState()["TargetSpotted"];
	const bool bWithinFiringRange = EnemyBeliefs->GetBeliefsState()["WithinRange"];
	const bool bAttackingTarget = !EnemyBeliefs->GetBeliefsState()["AttackingTarget"];
	const bool bHasFullHealth = EnemyBeliefs->GetBeliefsState()["HasFullHealth"];
	return bTargetSpotted && bHasFullHealth && bWithinFiringRange && bAttackingTarget;
}

void UChargeAttackAction::PerformAction()
{
	if(AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOuter()->GetOuter()))
		EnemyCharacter->TickEngage();
}

bool UChargeAttackAction::IsActionComplete() const
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(EnemyAgent->GetBeliefs());
	const bool bNoTarget = !EnemyBeliefs->GetBeliefsState()["TargetSpotted"];
	const bool bOutOfRange = !EnemyBeliefs->GetBeliefsState()["WithinRange"];
	const bool bInDangerOfDeath = EnemyBeliefs->GetBeliefsState()["InDangerOfDeath"];
	const bool bCantEngage = bNoTarget && bOutOfRange;
	
	return bCantEngage || bInDangerOfDeath;
}

void UChargeAttackAction::ApplyEffects(UWorldState& WorldState)
{
	Super::ApplyEffects(WorldState);
}
