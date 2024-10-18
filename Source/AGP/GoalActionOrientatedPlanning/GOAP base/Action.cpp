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
	    // Check if all bool preconditions are met
    for (const TPair<FString, bool>& Precondition : Preconditions)
    {
        // First check the beliefs if they contain the precondition
        if (Beliefs.GetBeliefsState().Contains(Precondition.Key))
        {
            if (Beliefs.GetBeliefsState()[Precondition.Key] != Precondition.Value)
            {
                // Precondition not met in the belief state, action is not possible
                return false;
            }
        }
        // If not found in beliefs, check the world state
        else if (WorldState.GetWorldState().Contains(Precondition.Key))
        {
            if (WorldState.GetWorldState()[Precondition.Key] != Precondition.Value)
            {
                // Precondition not met in the world state, action is not possible
                return false;
            }
        }
        else
        {
            // If neither the beliefs nor the world state contain the precondition, action is not possible
            return false;
        }
    }

    // Check if all vector preconditions are met
    for (const TPair<FString, FVector>& PreconditionVector : PreconditionsVectors)
    {
        // First check the beliefs for vector preconditions
        if (Beliefs.GetBeliefsStateVectors().Contains(PreconditionVector.Key))
        {
            if (Beliefs.GetBeliefsStateVectors()[PreconditionVector.Key] != PreconditionVector.Value)
            {
                // Vector precondition not met in the belief state, action is not possible
                return false;
            }
        }
        // If not found in beliefs, check the world state
        else if (WorldState.GetWorldVectorState().Contains(PreconditionVector.Key))
        {
            if (WorldState.GetWorldVectorState()[PreconditionVector.Key] != PreconditionVector.Value)
            {
                // Vector precondition not met in the world state, action is not possible
                return false;
            }
        }
        else
        {
            // If neither beliefs nor world state contain the vector precondition, action is not possible
            return false;
        }
    }

    // If all preconditions are satisfied, the action is possible
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
