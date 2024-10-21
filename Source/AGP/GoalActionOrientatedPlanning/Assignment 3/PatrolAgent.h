// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"   
#include "PatrolAgentBeliefs.h" 
#include "AGP/GoalActionOrientatedPlanning/GOAP base/Agent.h"
#include "PatrolAgent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AGP_API UPatrolAgent : public UAgent
{
	GENERATED_BODY()

public:
	UPatrolAgent();
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool PlanActions();
	void PerformAction();
	AActor* GetNextWaypoint();
	void MoveToLocation( FVector& location);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	TArray<AActor*> Waypoints;
	UPROPERTY(EditAnywhere)
	AActor* CurrentWayPoint;

	UPROPERTY(VisibleAnywhere)
	int currentIndex;
	UPROPERTY(VisibleAnywhere)
	AActor* TargetActor;

	UPROPERTY(VisibleAnywhere)
	AActor* TargetActorCopy;

	virtual UPatrolAgentBeliefs* GetBeliefs() const;
	virtual UPatrolAgentBeliefs* GetBeliefs() override;
	virtual void PerformDetection() override;
	bool inLineOfSight();
	void bDetermineLineOfSight();
	
protected:
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& Actors);
	virtual void BeginPlay() override;

	
};
