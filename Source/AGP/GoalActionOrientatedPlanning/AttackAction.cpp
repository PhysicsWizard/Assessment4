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
	//const bool bAttackingTarget = EnemyBeliefs->GetBeliefsState()["AttackingTarget"];

	UE_LOG(LogTemp, Log, TEXT("AttackAction - TargetSpotted?: %s, WithinFiringRange?: %s"),
bTargetSpotted ? TEXT("true") : TEXT("false"),
bWithinFiringRange ? TEXT("true") : TEXT("false"));

	
	//return bTargetSpotted && bWithinFiringRange && bAttackingTarget;
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
	const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(EnemyAgent->GetBeliefs());
	const bool bNoTarget = !EnemyBeliefs->GetBeliefsState()["TargetSpotted"];
	const bool bOutOfRange = !EnemyBeliefs->GetBeliefsState()["WithinRange"];
	const bool bInDangerOfDeath = EnemyBeliefs->GetBeliefsState()["InDangerOfDeath"];
	const bool bCantEngage = bNoTarget && bOutOfRange;
	 
	if (bCantEngage || bInDangerOfDeath)
	{
		// Reset AttackingTarget to false
		EnemyBeliefs->GetBeliefsState()["AttackingTarget"] = false;
		return true;
	}
	return false;
}

void UAttackAction::ApplyEffects(UWorldState& WorldState)
{
	Super::ApplyEffects(WorldState);
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(EnemyAgent->GetBeliefs());
	EnemyBeliefs->GetBeliefsState()["AttackingTarget"] = true;
}
