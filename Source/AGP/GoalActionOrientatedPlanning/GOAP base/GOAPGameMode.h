// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GOAPGameMode.generated.h"

class UAgent;
class UWorldState;
/**
 * 
 */
UCLASS()
class AGP_API AGOAPGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	// Constructor
	AGOAPGameMode(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private: 
	UPROPERTY(VisibleAnywhere)
	UWorldState* WorldState; 
	UPROPERTY(VisibleAnywhere)
	TArray<UAgent*> ActiveAgents;
public:

	// Function to register an agent in the game mode
	void RegisterAgent(UAgent* NewAgent);

	// Function to get the current world state
	UWorldState* GetWorldState() const;

	// Tick function to allow GameMode to run logic per frame
	virtual void Tick(float DeltaSeconds) override;
};
