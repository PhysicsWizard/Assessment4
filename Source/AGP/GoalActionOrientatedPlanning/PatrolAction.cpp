// Fill out your copyright notice in the Description page of Project Settings.
#include "AGP/GoalActionOrientatedPlanning/PatrolAction.h"

#include "EnemyAgent.h"
#include "AGP/Characters/EnemyCharacter.h"
#include "EnemyAgentBeliefs.h"

UPatrolAction::UPatrolAction()
{
	cost = 12.0f;
	//kinda like a default state
}

bool UPatrolAction::IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());

	bool bNoTargetSpotted = !EnemyAgent->GetBeliefs()->GetBeliefsState()["TargetSpotted"];
	bool bNotInDangerOfDeath = !EnemyAgent->GetBeliefs()->GetBeliefsState()["InDangerOfDeath"];

	UE_LOG(LogTemp, Log, TEXT("PatrolAction - bNoTargetSpotted: %s, bNotInDangerOfDeath: %s"),
		bNoTargetSpotted ? TEXT("true") : TEXT("false"),
		bNotInDangerOfDeath ? TEXT("true") : TEXT("false"));

	return bNoTargetSpotted && bNotInDangerOfDeath;
}

void UPatrolAction::PerformAction()
{
	if(AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOuter()->GetOuter()))
		EnemyCharacter->TickPatrol();
}

bool UPatrolAction::IsActionComplete() const
{
	/*
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(EnemyAgent->GetBeliefs());
	const bool bTargetSpotted = EnemyBeliefs->GetBeliefsState()["TargetSpotted"];
	const bool bInDangerOfDeath = EnemyBeliefs->GetBeliefsState()["InDangerOfDeath"];
	return bTargetSpotted && bInDangerOfDeath;
	*/
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	if(EnemyAgent->GetEnemyCharacterComponent()->GetSensedCharacter())
	{
		return true;
	}
	return false;
}

void UPatrolAction::ApplyEffects(UWorldState& WorldState)
{
	Super::ApplyEffects(WorldState);
}
