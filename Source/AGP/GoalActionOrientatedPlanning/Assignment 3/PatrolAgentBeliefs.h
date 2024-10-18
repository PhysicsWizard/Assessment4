// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"  
#include "AGP/GoalActionOrientatedPlanning/GOAP base/Beliefs.h"
#include "PatrolAgentBeliefs.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API UPatrolAgentBeliefs : public UBeliefs
{
	GENERATED_BODY()

protected: 
	// Track the last known location of the thief
	UPROPERTY(VisibleAnywhere)
	FVector ThiefLastKnownLocation;
	UPROPERTY(VisibleAnywhere)
	FVector ThiefCurrentLocation;

public:
	UPatrolAgentBeliefs();
	bool IsThiefVisible() const;
	void SetThiefVisible(bool bVisibility, FVector bPosition);
	FVector GetThiefCurrentLocation() const;
	void SetThiefCurrentLocation(FVector Location);
	FVector GetThiefsLastKnownLocation() const;
	void SetThiefsLastKnownLocation(FVector Location);
	virtual UPatrolAgentBeliefs* Clone() override;
};
