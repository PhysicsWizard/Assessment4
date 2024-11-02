// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h" 
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Agent.generated.h"


class UAISenseConfig_Sight;
class UAIPerceptionComponent;
class AAIController;
class UAgentSensor;
class UGoal;
class UAction;
class UWorldState;
class UBeliefs;
class UPlanner;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AGP_API UAgent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAgent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//variables
	UPROPERTY(VisibleAnywhere, Category = "GOAP")
	TArray<UGoal*> Goals;
	
	UPROPERTY(EditAnywhere, Category = "GOAP")
	TArray<UAction*> AvailableAction;

	UPROPERTY(VisibleAnywhere, Category = "GOAP")
	TArray<UAction*> CurrentPlan;

	UPROPERTY(VisibleAnywhere, Category = "GOAP")
	UWorldState* WorldState;

	UPROPERTY(VisibleAnywhere, Category = "GOAP")
	UBeliefs* Beliefs;

	UPROPERTY()
	UPlanner* Planner;

	UPROPERTY()
	UAgentSensor* Sensor;

	UPROPERTY()
	AAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	float DetectionRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	float RunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	float NormalSpeed;
	
	UPROPERTY()
	UAIPerceptionComponent* PerceptionComponent;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;
	
	UPROPERTY()
	UAIPerceptionStimuliSourceComponent* StimuliSourceComponent;

	
public:	
	// Called every frame
	UPROPERTY(VisibleAnywhere, Category = "GOAP")
	UGoal* CurrentGoal;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual bool PlanActions();
	virtual void PerformAction();
	bool GoalAchieved();
	void UpdateBelief(const FString& Key, bool Value);
	void UpdateBelief(const FString& Key, FVector Value);
	void DebugDrawSightPerception();
	UGoal* GetCurrentGoal();
	TArray<UAction*> GetAvailableAction();
	TArray<UAction*> GetCurrentPlan();
	TArray<UGoal*> GetGoals();
	UWorldState* GetWorldState();
	virtual UBeliefs& GetBeliefsConst() const;
	virtual UBeliefs* GetBeliefs();
	AAIController* GetAaiController();
	float GetNormalSpeed();
	float GetRunSpeed();
	virtual void PerformDetection();
		
};
