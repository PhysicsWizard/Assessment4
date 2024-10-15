// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "StealAction.generated.h"

class UPatrolAgentBeliefs;
class UPatrolAgent;
class AAIController;
/**
 * 
 */
UCLASS()
class NAVMESH_TUTORIAL_API UStealAction : public UAction
{
	GENERATED_BODY()
public:
	UStealAction(); 
	// Check if chasing the thief is possible (is the thief visible?)
	virtual bool IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)override;

	// Chase the thief (move towards the last known location)
	virtual void PerformAction() override;

	// Check if the action is complete (e.g., lost sight of thief)
	virtual bool IsActionComplete() const override;

	// Apply effects to the world state (e.g., thief is no longer visible after chase)
	virtual void ApplyEffects(UWorldState& WorldState) override;
	
protected:
	UPROPERTY()
	AActor* OwnerActor; 
	UPROPERTY()
	AAIController* AIController;
	UPROPERTY()
	UPatrolAgent* AgentComponent;
};
