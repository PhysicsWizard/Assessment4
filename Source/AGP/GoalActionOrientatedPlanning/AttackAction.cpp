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
	const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(EnemyAgent->GetBeliefs());
	const bool bTargetSpotted = EnemyBeliefs->GetBeliefsState()["TargetSpotted"];
	const bool bWithinFiringRange = EnemyBeliefs->GetBeliefsState()["WithinRange"];
	const bool bAttackingTarget = !EnemyBeliefs->GetBeliefsState()["AttackingTarget"];
	return bTargetSpotted && bWithinFiringRange && bAttackingTarget;
}

void UAttackAction::PerformAction()
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOuter()->GetOuter());
	EnemyCharacter->TickEngageStationary();
}

bool UAttackAction::IsActionComplete() const
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(EnemyAgent->GetBeliefs());
	const bool bNoTarget = !EnemyBeliefs->GetBeliefsState()["TargetSpotted"];
	const bool bOutOfRange = !EnemyBeliefs->GetBeliefsState()["WithinRange"];
	const bool bInDangerOfDeath = EnemyBeliefs->GetBeliefsState()["InDangerOfDeath"];
	const bool bCantEngage = bNoTarget && bOutOfRange;
	
	return bCantEngage || bInDangerOfDeath;
}

void UAttackAction::ApplyEffects(UWorldState& WorldState)
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(EnemyAgent->GetBeliefs());
	EnemyBeliefs->GetBeliefsState()["AttackingTarget"] = true;
}
