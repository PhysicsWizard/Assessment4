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
	/*
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(EnemyAgent->GetBeliefs());
	const bool bTargetSpotted = EnemyBeliefs->GetBeliefsState()["TargetSpotted"];
	const bool bWithinFiringRange = EnemyBeliefs->GetBeliefsState()["WithinRange"];
	//const bool bAttackingTarget = !EnemyBeliefs->GetBeliefsState()["AttackingTarget"];
	const bool bHasFullHealth = EnemyBeliefs->GetBeliefsState()["HasFullHealth"];

	UE_LOG(LogTemp, Log, TEXT("AttackAction - TargetSpotted?: %s, WithinFiringRange?: %s, HasFullHealth?: %s"),
	bTargetSpotted ? TEXT("true") : TEXT("false"),
	bWithinFiringRange ? TEXT("true") : TEXT("false"),
	bHasFullHealth ? TEXT("true") : TEXT("false"));
	
	//return bTargetSpotted && bHasFullHealth && bWithinFiringRange && bAttackingTarget;
	return bTargetSpotted && bHasFullHealth && bWithinFiringRange;
	*/
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	if(EnemyAgent->GetEnemyCharacterComponent()->GetSensedCharacter())
	{
		return true;
	}
	return false;
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
	
	if (bCantEngage || bInDangerOfDeath)
	{
		// Reset AttackingTarget to false
		EnemyBeliefs->GetBeliefsState()["AttackingTarget"] = false;
		return true;
	}
	return false;
}

void UChargeAttackAction::ApplyEffects(UWorldState& WorldState)
{
	Super::ApplyEffects(WorldState);
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(EnemyAgent->GetBeliefs());
	EnemyBeliefs->GetBeliefsState()["AttackingTarget"] = true;
}
