// Fill out your copyright notice in the Description page of Project Settings.
#include "AGP/GoalActionOrientatedPlanning/ChargeAttackAction.h"

#include "EnemyAgent.h"
#include "AGP/Characters/EnemyCharacter.h"

UChargeAttackAction::UChargeAttackAction()
{
	cost = 7.0f;
	Effects.Add("AttackingTarget", true);
}

bool UChargeAttackAction::IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter()); 
	const bool bTargetSpotted = EnemyAgent->GetBeliefs()->GetBeliefsState()["TargetSpotted"];
	const bool bWithinFiringRange = EnemyAgent->GetBeliefs()->GetBeliefsState()["WithinRange"];
	const bool bNotInDangerOfDeath = EnemyAgent->GetBeliefs()->GetBeliefsState()["InDangerOfDeath"] == false;
	const bool bHasHealthToTankWhileCharging = EnemyAgent->GetBeliefs()->GetBeliefsState()["HasFullHealth"] == true;
	return bTargetSpotted && bWithinFiringRange && bNotInDangerOfDeath && bHasHealthToTankWhileCharging;
}

void UChargeAttackAction::PerformAction()
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOuter()->GetOuter());
	EnemyCharacter->TickEngage();
	UE_LOG(LogTemp, Warning, TEXT("Engaging in offensive posture...")); 
}

bool UChargeAttackAction::IsActionComplete() const
{  
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter()); 
	const bool bNoTarget = EnemyAgent->GetBeliefs()->GetBeliefsState()["TargetSpotted"] == false;
	const bool bOutOfRange = EnemyAgent->GetBeliefs()->GetBeliefsState()["WithinRange"] == false;
	const bool bInDangerOfDeath = EnemyAgent->GetBeliefs()->GetBeliefsState()["InDangerOfDeath"] == true;
	
	if (bNoTarget || bOutOfRange|| bInDangerOfDeath)
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
	WorldState.GetWorldState()["AllEnemiesEliminated"] = true;
}
