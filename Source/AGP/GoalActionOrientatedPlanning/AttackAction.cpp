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
	const bool bNotInDangerOfDeath = EnemyAgent->GetBeliefs()->GetBeliefsState()["InDangerOfDeath"] == false;
	const bool bDoesNotHaveHealthToTankWhileCharging = EnemyAgent->GetBeliefs()->GetBeliefsState()["HasFullHealth"] == false;
	return bDoesNotHaveHealthToTankWhileCharging && bTargetSpotted && bWithinFiringRange && bNotInDangerOfDeath;
}

void UAttackAction::PerformAction()
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOuter()->GetOuter());
	EnemyCharacter->TickEngageStationary()();
	UE_LOG(LogTemp, Warning, TEXT("Engaging in defensive posture...")); 
}

bool UAttackAction::IsActionComplete() const
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

void UAttackAction::ApplyEffects(UWorldState& WorldState)
{
	Super::ApplyEffects(WorldState);
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter()); 
	WorldState.GetWorldState()["AllEnemiesEliminated"] = true;
}
