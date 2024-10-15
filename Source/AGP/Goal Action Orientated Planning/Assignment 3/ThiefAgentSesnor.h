// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AgentSensor.h"
#include "ThiefAgent.h"
#include "ThiefAgentSesnor.generated.h"

/**
 * 
 */
UCLASS()
class NAVMESH_TUTORIAL_API UThiefAgentSesnor : public UAgentSensor
{
	GENERATED_BODY()

public:
	virtual void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;
	virtual void UpdateBeliefsFromSensors() override;
	virtual UThiefAgent* GetTheOwner() override;
	void SetDetectionRange(float range);

protected:
	UPROPERTY()
	FVector GuardPosition;
};
