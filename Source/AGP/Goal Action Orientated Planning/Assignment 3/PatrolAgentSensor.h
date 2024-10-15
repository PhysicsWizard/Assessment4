// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AgentSensor.h"
#include "PatrolAgent.h"
#include "PatrolAgentSensor.generated.h"

class UPatrolAgent;
/**
 * 
 */
UCLASS()
class NAVMESH_TUTORIAL_API UPatrolAgentSensor : public UAgentSensor
{
	GENERATED_BODY()

public:
	virtual void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;
	virtual void UpdateBeliefsFromSensors() override;
	virtual UPatrolAgent* GetTheOwner() override;
	void SetDetectionRange(float range);

protected:
	UPROPERTY()
	FVector ThiefPosition;
	
	
};
