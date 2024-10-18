// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldState.h"

UWorldState* UWorldState::Instance = nullptr;

TMap<FString, bool> UWorldState::GetWorldState()
{
	return States;
}

TMap<FString, FVector> UWorldState::GetWorldVectorState()
{
	return VectorStates;
}

TMap<FString, bool> UWorldState::GetWorldState() const 
{
	return States;
}

TMap<FString, FVector> UWorldState::GetWorldVectorState() const
{
	return VectorStates;
}

void UWorldState::UpdateState(const TMap<FString, bool>& Effects)
{
	for (const TPair<FString, bool>& Effect : Effects)
	{
		States.Add(Effect.Key, Effect.Value);
	}
}

void UWorldState::UpdateVectorState(const TMap<FString, FVector>& Effects)
{
	for (const TPair<FString, FVector>& Effect : Effects)
	{
		VectorStates.Add(Effect.Key, Effect.Value);
	}
}

void UWorldState::SetState(FString key, bool value)
{
	if(States.Contains(key))
		States[key] = value;
	else
		States.Add(key, value);
}

void UWorldState::SetVectorState(FString key, FVector value)
{
	if(VectorStates.Contains(key))
		VectorStates[key] = value;
	else
		VectorStates.Add(key, value);
}

UWorldState* UWorldState::GetInstance()
{
	if (!Instance)
	{ 
		Instance = NewObject<UWorldState>(GetTransientPackage(), UWorldState::StaticClass());
	}
	return Instance;
}

void UWorldState::PrintWorldStates() const
{
	// For boolean states
	for (auto& worldState : States)
	{
		UE_LOG(LogTemp, Warning, TEXT("Key: %s, Value: %s"), *worldState.Key, worldState.Value ? TEXT("True") : TEXT("False"));
	}

	// For vector states
	for (auto& worldState : VectorStates)
	{
		UE_LOG(LogTemp, Warning, TEXT("Key: %s, Value: %s"), *worldState.Key, *worldState.Value.ToString());
	}
}

UWorldState::UWorldState()
{
	States.Add("GoldTaken", false);
	States.Add("ThiefVisible", false);
	States.Add("GuardVisible", false);
	States.Add("Escaped", false);
	VectorStates.Add("ThiefLastKnownPosition", FVector::ZeroVector);
	VectorStates.Add("PatrolAgentPosition", FVector::ZeroVector);
	VectorStates.Add("ThiefAgentLocation", FVector::ZeroVector); 
	
	//PrintWorldStates();
}

void UWorldState::BeginDestroy()
{
	UObject::BeginDestroy();
	Instance = nullptr;
}

UWorldState* UWorldState::Clone() 
{
	UWorldState* NewWorldState = NewObject<UWorldState>();
	NewWorldState->States = this->States;  // Assuming States is a TMap or similar
	NewWorldState->VectorStates = this->VectorStates;  // Deep copy if needed
	return NewWorldState;
}
