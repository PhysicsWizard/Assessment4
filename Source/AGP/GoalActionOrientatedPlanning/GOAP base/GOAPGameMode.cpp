// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAPGameMode.h"

#include "WorldState.h"

AGOAPGameMode::AGOAPGameMode(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// Properly create WorldState as a default subobject
	WorldState = ObjectInitializer.CreateDefaultSubobject<UWorldState>(this, TEXT("WorldState"));
}

void AGOAPGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AGOAPGameMode::RegisterAgent(UAgent* NewAgent)
{
	if(NewAgent)
	{
		if(!ActiveAgents.Contains(NewAgent))
			ActiveAgents.Add(NewAgent);
	}
}

UWorldState* AGOAPGameMode::GetWorldState() const
{
	return WorldState;
}

void AGOAPGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
