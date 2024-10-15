// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Agent.h"
#include "ThiefAgentBeliefs.h"
#include "ThiefAgent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NAVMESH_TUTORIAL_API UThiefAgent : public UAgent
{
	GENERATED_BODY()

public:
	UThiefAgent();
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool PlanActions();
	void PerformAction();
	
	virtual UThiefAgentBeliefs* GetBeliefs() const;
	virtual UThiefAgentBeliefs* GetBeliefs() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	AActor* Gold;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	TArray<AActor*> Waypoints;

	UPROPERTY()
	AActor* WayPoint;

	void MoveToLocation(FVector& location);
	FVector GetGoldLocation();
	virtual void PerformDetection() override;
	bool InLOS();
	FVector ClosestPoint();
	FVector FurthestPoint();
protected:
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& Actors);
	virtual void BeginPlay() override;
	
	
	
};
