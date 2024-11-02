// Fill out your copyright notice in the Description page of Project Settings.
#include "AGP/GoalActionOrientatedPlanning/RetreatAction.h"

#include "EnemyAgent.h"
#include "EnemyAgentBeliefs.h"
#include "AGP/Characters/EnemyCharacter.h"

URetreatAction::URetreatAction()
{
	cost = 1.0f;
	Preconditions.Add("InDangerOfDeath", true);
	Effects.Add("InDangerOfDeath", false);
	Effects.Add("TargetSpotted", false);
	Effects.Add("SafeDistanceToHeal", true);
	Effects.Add("AttackingTarget", false);
}

bool URetreatAction::IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(EnemyAgent->GetBeliefs());
	const bool bAlmostAtHalfHealth = EnemyBeliefs->bWithinHealthRangeTolerance(0.61f);
	const bool bEnemyTooClose = EnemyBeliefs->bIsClose();
	const bool bEnemyVisible = EnemyBeliefs->GetTarget() !=nullptr;

	return bAlmostAtHalfHealth && bEnemyTooClose && bEnemyVisible;
}

void URetreatAction::PerformAction()
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	if(AEnemyCharacter* EnemeyCharacterComponent = EnemyAgent->GetEnemyCharacterComponent())
	{
		EnemeyCharacterComponent->TickEvade();
	}
}

bool URetreatAction::IsActionComplete() const
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(EnemyAgent->GetBeliefs());
	const bool bIsEnemyFarEnough = !EnemyBeliefs->bIsClose();
	const bool bNoEnemyVisible = EnemyBeliefs->GetTarget() == nullptr;
	return bIsEnemyFarEnough && bNoEnemyVisible;
}

void URetreatAction::ApplyEffects(UWorldState& WorldState)
{
	Super::ApplyEffects(WorldState);
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter()); 
	EnemyAgent->GetBeliefs()->GetBeliefsState()["InDangerOfDeath"] = false;
	EnemyAgent->GetBeliefs()->GetBeliefsState()["TargetSpotted"] = false;
	EnemyAgent->GetBeliefs()->GetBeliefsState()["SafeDistanceToHeal"] = true;
}
