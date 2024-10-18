// Fill out your copyright notice in the Description page of Project Settings.


#include "ThiefAgentBeliefs.h"

bool UThiefAgentBeliefs::IsPatrolAgentVisible() const
{
	return BeliefsState.Contains("GuardVisible") && BeliefsState["GuardVisible"] &&
	BeliefsStateVectors.Contains("GuardPosition") && BeliefsStateVectors["GuardPosition"] != FVector::Zero();
}

void UThiefAgentBeliefs::SetPatrolAgentVisible(bool bVisibility , FVector bPosition)
{
	BeliefsState.Add("GuardVisible", bVisibility);
	if(BeliefsState["GuardVisible"])
	{
		BeliefsStateVectors["GuardPosition"] = bPosition;
		PatrolAgentCurrentLocation = bPosition;
	}
	else
		BeliefsStateVectors["GuardPosition"] = FVector::Zero();
}

FVector UThiefAgentBeliefs::GetPatrolAgentCurrentLocation() const
{
	return PatrolAgentCurrentLocation;
}


void UThiefAgentBeliefs::SetPatrolAgentCurrentLocation(FVector Location)
{
	PatrolAgentCurrentLocation = Location;
}

FVector UThiefAgentBeliefs::GetThiefsLastKnownLocation() const
{
	return PatrolAgentLastKnownLocation;
}

void UThiefAgentBeliefs::SetThiefsLastKnownLocation(FVector Location)
{
	PatrolAgentLastKnownLocation = Location;
}

UThiefAgentBeliefs* UThiefAgentBeliefs::Clone()
{ 
	UThiefAgentBeliefs* Clone = NewObject<UThiefAgentBeliefs>();
	Clone->BeliefsState = this->BeliefsState;  
	Clone->BeliefsStateVectors = this->BeliefsStateVectors;  
	return Clone; 
}

UThiefAgentBeliefs::UThiefAgentBeliefs()
{ 
	BeliefsState.Add("GuardVisible", false);
	BeliefsStateVectors.Add("GuardPosition", FVector::ZeroVector);
	BeliefsStateVectors.Add("GuardLastKnownPosition", FVector::ZeroVector);
}
