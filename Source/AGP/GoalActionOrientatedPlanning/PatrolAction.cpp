// Fill out your copyright notice in the Description page of Project Settings.
#include "AGP/GoalActionOrientatedPlanning/PatrolAction.h"
#include "AGP/Characters/EnemyCharacter.h"
#include "EnemyAgentBeliefs.h"

UPatrolAction::UPatrolAction()
{
	cost = 12.0f;
	//kinda like a default state
}

bool UPatrolAction::IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)
{
	UEnemyAgentBeliefs* EnemyBeliefs = nullptr;
	if(UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter()))
	{
		EnemyBeliefs = EnemyAgent->GetBeliefs();
	}
	if (!EnemyBeliefs)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyAgent Beliefs null!"));
		return false; // Null check
	}
	const auto& BeliefsState = EnemyBeliefs->GetBeliefsState();
	const bool bNoTargetSpotted = BeliefsState.Contains("TargetSpotted") && !BeliefsState["TargetSpotted"];
	const bool bNotInDangerOfDeath = BeliefsState.Contains("InDangerOfDeath") && !BeliefsState["InDangerOfDeath"];
	return bNoTargetSpotted && bNotInDangerOfDeath;
}

void UPatrolAction::PerformAction()
{
	if(AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOuter()->GetOuter()))
		EnemyCharacter->TickPatrol();
}

bool UPatrolAction::IsActionComplete() const
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(EnemyAgent->GetBeliefs());
	const bool bTargetSpotted = EnemyBeliefs->GetBeliefsState()["TargetSpotted"];
	const bool bInDangerOfDeath = EnemyBeliefs->GetBeliefsState()["InDangerOfDeath"];
	return bTargetSpotted && bInDangerOfDeath;
}

void UPatrolAction::ApplyEffects(UWorldState& WorldState)
{
	Super::ApplyEffects(WorldState);
}
