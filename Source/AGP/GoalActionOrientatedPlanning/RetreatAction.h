// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAP base/Action.h"
#include "RetreatAction.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API URetreatAction : public UAction
{
	GENERATED_BODY()
	URetreatAction();
	virtual bool IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)override;
	virtual void PerformAction() override;
	virtual bool IsActionComplete() const override;
	virtual void ApplyEffects(UWorldState& WorldState) override;
};
