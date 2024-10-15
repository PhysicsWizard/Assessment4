// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Beliefs.h"
#include "ThiefAgentBeliefs.generated.h"

/**
 * 
 */
UCLASS()
class NAVMESH_TUTORIAL_API UThiefAgentBeliefs : public UBeliefs
{
	GENERATED_BODY()
protected: 
	// Track the last known location of the thief
	UPROPERTY(VisibleAnywhere)
	FVector PatrolAgentLastKnownLocation;

	UPROPERTY(VisibleAnywhere)
	FVector PatrolAgentCurrentLocation;

public:
	bool IsPatrolAgentVisible() const;
	void SetPatrolAgentVisible(bool bVisibility, FVector bPosition);
	FVector GetPatrolAgentCurrentLocation() const;
	void SetPatrolAgentCurrentLocation(FVector Location);
	FVector GetThiefsLastKnownLocation() const;
	void SetThiefsLastKnownLocation(FVector Location);
	virtual UThiefAgentBeliefs* Clone() override;

protected:
	UThiefAgentBeliefs();
};
