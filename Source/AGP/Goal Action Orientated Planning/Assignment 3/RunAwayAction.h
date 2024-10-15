// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "ThiefAgentBeliefs.h"
#include "RunAwayAction.generated.h"

/**
 * 
 */
UCLASS()
class NAVMESH_TUTORIAL_API URunAwayAction : public UAction
{
	GENERATED_BODY()

public:
	URunAwayAction();
	
	// Check if the agent can investigate (is there a last known location?)
	virtual bool IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)override;

	// Investigate the last known location
	virtual void PerformAction() override;

	// Check if the action is complete (e.g., investigation is finished)
	virtual bool IsActionComplete() const override;

	// Apply effects to the world state (e.g., after investigating)
	virtual void ApplyEffects(UWorldState& WorldState) override;
};
