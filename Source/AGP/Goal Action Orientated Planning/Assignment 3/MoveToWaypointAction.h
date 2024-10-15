// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "MoveToWaypointAction.generated.h"

class UPatrolAgentBeliefs;
/**
 * 
 */
UCLASS()
class NAVMESH_TUTORIAL_API UMoveToWaypointAction : public UAction
{
	GENERATED_BODY()

public:
	UMoveToWaypointAction(); 
	virtual bool IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs) override;
	virtual void ApplyEffects (UWorldState& WorldState)override;
	virtual bool IsActionComplete() const override;
	virtual void PerformAction()override;
};
