// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"     
#include "AGP/GoalActionOrientatedPlanning/GOAP base/Action.h"
#include "InvestigateAreaAction.generated.h"

class UPatrolAgentBeliefs;
/**
 * 
 */
UCLASS()
class AGP_API UInvestigateAreaAction : public UAction
{
	GENERATED_BODY()

public:
	UInvestigateAreaAction();
	
	// Check if the agent can investigate (is there a last known location?)
	virtual bool IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs) override;
	FVector GetRandomPoint();

	// Investigate the last known location
	virtual void PerformAction() override;

	// Check if the action is complete (e.g., investigation is finished)
	virtual bool IsActionComplete() const override;

	// Apply effects to the world state (e.g., after investigating)
	virtual void ApplyEffects(UWorldState& WorldState) override;

protected:
	UPROPERTY()
	bool bDelayActive = true;
	UPROPERTY()
	bool bInvestigateArea = false;
	UPROPERTY()
	bool bInvestigationDone = false;
	UPROPERTY()
	FVector RandomPosition;
	
};
