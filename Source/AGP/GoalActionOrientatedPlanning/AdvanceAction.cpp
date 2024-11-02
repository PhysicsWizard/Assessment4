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
	const bool bTargetSpotted = EnemyAgent->GetBeliefs()->GetBeliefsState()["TargetSpotted"];
	const bool bHasTargetPosition = EnemyAgent->GetBeliefs()->GetBeliefsStateVectors()["TargetPosition"] != FVector::ZeroVector;
	const bool bHasTargetToAdvanceTo = bTargetSpotted && bHasTargetPosition;

	//if no target is spotted, check memory/ last known location
	const bool bHasValidLastKnownLocation = EnemyAgent->GetBeliefs()->GetBeliefsStateVectors()["LastKnownTargetPosition"] != FVector::ZeroVector;

	return bHasTargetToAdvanceTo || bHasValidLastKnownLocation;
}

void UAdvanceAction::PerformAction()
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOuter()->GetOuter());
	EnemyCharacter->TickAdanceToTarget();
	
}

bool UAdvanceAction::IsActionComplete() const
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOuter()->GetOuter());
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter()); 
	// target not seen by agent itself
	const bool bNoTargetSpotted = !EnemyAgent->GetBeliefs()->GetBeliefsState()["TargetSpotted"];
	const bool bHasNoTargetPosition = EnemyAgent->GetBeliefs()->GetBeliefsStateVectors()["TargetPosition"] == FVector::ZeroVector;
	const bool bHasNoTarget = bHasNoTargetPosition && bNoTargetSpotted;

	//check if within proper distance
	const float DistanceToSeenTarget = FVector::Dist(EnemyCharacter->GetActorLocation(),EnemyAgent->GetBeliefs()->GetBeliefsStateVectors()["TargetPosition"]);
	const float DistanceToLastKnownPositionOfTarget = FVector::Dist(EnemyCharacter->GetActorLocation(),EnemyAgent->GetBeliefs()->GetBeliefsStateVectors()["LastKnownTargetPosition"]);
	const bool bWithinDistanceOfTarget = DistanceToSeenTarget <= 200.0f;
	const bool bWithinDistanceOfLastKnownPostion = DistanceToLastKnownPositionOfTarget <= 200.0f;
	
	
	return bHasNoTarget || bWithinDistanceOfTarget || bWithinDistanceOfLastKnownPostion;
}

void UAdvanceAction::ApplyEffects(UWorldState& WorldState)
{
	Super::ApplyEffects(WorldState);
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter()); 
	EnemyAgent->GetBeliefs()->GetBeliefsState()["WithinRange"] = true; 
}
