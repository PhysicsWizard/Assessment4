// Fill out your copyright notice in the Description page of Project Settings.


#include "ThiefAgentSesnor.h"

#include "PatrolAgent.h"

void UThiefAgentSesnor::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::OnPerceptionUpdated(UpdatedActors);
	//UpdateBeliefsFromSensors();
}

void UThiefAgentSesnor::UpdateBeliefsFromSensors()
{ 
	for (AActor* Actor : DetectedActors)
	{
		if (Actor->IsA(UPatrolAgent::StaticClass()))
		{ 
			GuardPosition = Actor->GetActorLocation();
			UpdateBelief("GuardVisib le", true);
			UpdateBelief("GuardPosition", GuardPosition);
			UpdateBelief("GuardLastKnownPosition", GuardPosition);
		}
		else
		{ 
			GuardPosition = FVector::ZeroVector;
			UpdateBelief("GuardVisible", false);
			UpdateBelief("GuardPosition", GuardPosition);
		}
	}
}

UThiefAgent* UThiefAgentSesnor::GetTheOwner()
{
	return Cast<UThiefAgent>(OwnerAgent);
}

void UThiefAgentSesnor::SetDetectionRange(float range)
{
	DetectionRange = range;
}
