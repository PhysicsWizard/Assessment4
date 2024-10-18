// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolAgentBeliefs.h"

UPatrolAgentBeliefs::UPatrolAgentBeliefs()
{
	BeliefsState.Add("ThiefVisible", false);
	BeliefsState.Add("AtWaypoint", false);
	BeliefsStateVectors.Add("ThiefLastKnownPosition", FVector::ZeroVector);
	BeliefsStateVectors.Add("ThiefVisible", FVector::ZeroVector);
}

bool UPatrolAgentBeliefs::IsThiefVisible() const
{
	return BeliefsState.Contains("ThiefVisible") && BeliefsState["ThiefVisible"] &&
		BeliefsStateVectors.Contains("ThiefPosition") && BeliefsStateVectors["ThiefPosition"] != FVector::Zero();
}

void UPatrolAgentBeliefs::SetThiefVisible(bool bVisibility, FVector bPosition)
{
	BeliefsState.Add("ThiefVisible", bVisibility);
}

FVector UPatrolAgentBeliefs::GetThiefCurrentLocation() const
{
	return ThiefCurrentLocation;
}

void UPatrolAgentBeliefs::SetThiefCurrentLocation(FVector Location)
{
	ThiefCurrentLocation = Location;
	BeliefsStateVectors.Add("ThiefPosition", Location);
}

FVector UPatrolAgentBeliefs::GetThiefsLastKnownLocation() const
{
	return ThiefLastKnownLocation;
}

void UPatrolAgentBeliefs::SetThiefsLastKnownLocation(FVector Location)
{
	ThiefLastKnownLocation = Location;
	BeliefsStateVectors.Add("ThiefLastKnownPosition", Location);
}

UPatrolAgentBeliefs* UPatrolAgentBeliefs::Clone()
{
	
	UPatrolAgentBeliefs* Clone = NewObject<UPatrolAgentBeliefs>();
	Clone->BeliefsState = this->BeliefsState;  
	Clone->BeliefsStateVectors = this->BeliefsStateVectors;  
	return Clone;
	//return Super::Clone();
}
