// Fill out your copyright notice in the Description page of Project Settings.


#include "Beliefs.h"

TMap<FString, bool> UBeliefs::GetBeliefsState()
{
	return BeliefsState;
}

TMap<FString, bool> UBeliefs::GetBeliefsState() const
{
	return BeliefsState;
}

TMap<FString, FVector>  UBeliefs::GetBeliefsStateVectors() 
{
	return BeliefsStateVectors;
}

TMap<FString, FVector> UBeliefs::GetBeliefsStateVectors() const
{
	return BeliefsStateVectors;
}

void UBeliefs::UpdateBeliefsState(TMap<FString, bool> NewBeliefs)
{
	for (const TPair<FString, bool>& NewBelief : NewBeliefs)
	{
		BeliefsState.Add(NewBelief.Key, NewBelief.Value);
	}
}

void UBeliefs::UpdateBeliefsStateVectors(TMap<FString, FVector> NewBeliefs)
{
	for (const TPair<FString, FVector>& NewBelief : NewBeliefs)
	{
		BeliefsStateVectors.Add(NewBelief.Key, NewBelief.Value);
	}
}

bool UBeliefs::BeliefsMatchesWorldState( const UWorldState& WorldState)const 
{
	TMap<FString,bool> ActualWorldState = WorldState.GetWorldState();
	TMap<FString,FVector> ActualWorldStateVectors = WorldState.GetWorldVectorState();
	
	for (const TPair<FString, bool>& Belief  : BeliefsState)
	{
		if(ActualWorldState.Contains(Belief.Key))
			if(Belief.Value != ActualWorldState[Belief.Key])
				return false;
	}

	for (const TPair<FString, FVector>& Belief  : BeliefsStateVectors)
	{
		if(ActualWorldStateVectors.Contains(Belief.Key))
			if(Belief.Value != ActualWorldStateVectors[Belief.Key])
				return false;
	}
	
	return true;
}

UBeliefs* UBeliefs::Clone()
{
	UBeliefs* Clone = NewObject<UBeliefs>();
	Clone->BeliefsState = this->BeliefsState;  
	Clone->BeliefsStateVectors = this->BeliefsStateVectors;  
	return Clone;
}

