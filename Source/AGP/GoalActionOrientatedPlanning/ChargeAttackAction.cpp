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
	const bool bWithinFiringRange = EnemyAgent->GetBeliefs()->GetBeliefsState()["WithinRange"];
	const bool bHasFullHealth = EnemyAgent->GetBeliefs()->GetBeliefsState()["HasFullHealth"];

	UE_LOG(LogTemp, Log, TEXT("AttackAction - TargetSpotted?: %s, WithinFiringRange?: %s, HasFullHealth?: %s"),
	bTargetSpotted ? TEXT("true") : TEXT("false"),
	bWithinFiringRange ? TEXT("true") : TEXT("false"),
	bHasFullHealth ? TEXT("true") : TEXT("false"));
	
	return bTargetSpotted && bHasFullHealth && bWithinFiringRange; 
}

void UChargeAttackAction::PerformAction()
{
	if(AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOuter()->GetOuter()))
	{ 
		EnemyCharacter->TickEngageStationary();
	}
}

bool UChargeAttackAction::IsActionComplete() const
{
	 
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter()); 
	const bool bNoTarget = EnemyAgent->GetBeliefs()->GetBeliefsState()["TargetSpotted"] == false;
	const bool bOutOfRange = EnemyAgent->GetBeliefs()->GetBeliefsState()["WithinRange"] == false;
	const bool bInDangerOfDeath = EnemyAgent->GetBeliefs()->GetBeliefsState()["InDangerOfDeath"] == true;
	
	if (bNoTarget || bOutOfRange || bInDangerOfDeath)
	{ 
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
