// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"

TMap<FString, bool> UAction::GetPreconditions()
{
	return Preconditions;
}

TMap<FString, FVector> UAction::GetPreconditionsVectors()
{
	return PreconditionsVectors;
}

TMap<FString, bool> UAction::GetEffects()
{
	return Effects;
}

TMap<FString, FVector> UAction::GetEffectsVectors()
{
	return EffectVectors;
}

float UAction::Getcost()
{
	return cost;
}

bool UAction::IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)
{
    return true;
}

void UAction::ApplyEffects(UWorldState& WorldState)
{
	TMap<FString, bool> NewWorldState = WorldState.GetWorldState();
	TMap<FString, FVector> NewWorldStateVectors = WorldState.GetWorldVectorState();
	for(const TPair<FString, bool>& Effect : Effects )
	{
		NewWorldState.Add(Effect.Key, Effect.Value);
	}
	for(const TPair<FString, FVector>& Effect : EffectVectors )
	{
		NewWorldStateVectors.Add(Effect.Key, Effect.Value);
	}
	WorldState.UpdateState(NewWorldState);
	WorldState.UpdateVectorState(NewWorldStateVectors);
}

bool UAction::IsActionComplete() const
{
	return true;
}

void UAction::PerformAction()
{
}
