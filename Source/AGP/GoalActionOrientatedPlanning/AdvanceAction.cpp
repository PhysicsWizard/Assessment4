// Fill out your copyright notice in the Description page of Project Settings.
#include "AGP/GoalActionOrientatedPlanning/AdvanceAction.h"

#include "EnemyAgent.h"
#include "EnemyAgentBeliefs.h"
#include "AGP/Characters/EnemyCharacter.h"

UAdvanceAction::UAdvanceAction()
{
	cost = 1.0f;
	Effects.Add("WithinRange", true);
	Effects.Add("AttackingTarget", true); 
}

bool UAdvanceAction::IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter()); 
	//check if a target is directly spotted
	const bool bNoTargetSpotted = !EnemyAgent->GetBeliefs()->GetBeliefsState()["TargetSpotted"];
	const bool bLastKnownPosition =
		EnemyAgent->GetBeliefs()->GetBeliefsStateVectors()["LastKnownTargetPosition"] != FVector::ZeroVector;

	return bNoTargetSpotted && !bLastKnownPosition;
}

void UAdvanceAction::PerformAction()
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter()); 
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOuter()->GetOuter());
	EnemyCharacter->TickGoToLocation(EnemyAgent->GetBeliefs()->GetBeliefsStateVectors()["LastKnownTargetPosition"]);
	
}

bool UAdvanceAction::IsActionComplete() const
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOuter()->GetOuter());
	const float DistanceToLastKnownPosition =
		FVector::Dist(EnemyAgent->GetBeliefs()->GetBeliefsStateVectors()["LastKnownTargetPosition"],
		EnemyCharacter->GetActorLocation());
	
	//check if a target is directly spotted
	const bool bTargetSpotted = EnemyAgent->GetBeliefs()->GetBeliefsState()["TargetSpotted"];\

	return bTargetSpotted || DistanceToLastKnownPosition <= 25.0f;
}

void UAdvanceAction::ApplyEffects(UWorldState& WorldState)
{
	Super::ApplyEffects(WorldState);
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter()); 
	EnemyAgent->GetBeliefs()->GetBeliefsStateVectors()["LastKnownTargetPosition"] = FVector::ZeroVector;
}
