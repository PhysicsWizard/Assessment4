// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldState.h"
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h" 
#include "AgentSensor.generated.h"

class UAgent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AGP_API UAgentSensor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAgentSensor();
	
	UFUNCTION()
	virtual void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	TArray<AActor*> GetDetectedActors() const;

	virtual void UpdateBeliefsFromSensors();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	UAgent* OwnerAgent;

	UPROPERTY()
	UWorldState* WorldState;
	
	UPROPERTY()
	float DetectionRange;
	
	UPROPERTY()
	float PersistantSightRange;
    	
	UPROPERTY()
	float FieldOfView;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Belief system (you can replace this with your actual belief structure)
	UPROPERTY()
	TMap<FString, bool> Beliefs;
	void SetOwnerAgent(UAgent* Agent);
	virtual UAgent* GetTheOwner();

	// Array to store detected actors
	UPROPERTY()
	TArray<AActor*> DetectedActors;
	void UpdateBelief(const FString& Key, bool Value);
	void UpdateBelief(const FString& Key, FVector Value);
	

		
};
