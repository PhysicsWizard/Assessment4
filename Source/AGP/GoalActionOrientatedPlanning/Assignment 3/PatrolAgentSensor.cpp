// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolAgentSensor.h"

#include "ChaseThief.h"
#include "ThiefAgent.h"

void UPatrolAgentSensor::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::OnPerceptionUpdated(UpdatedActors);
	//UpdateBeliefsFromSensors();
}

void UPatrolAgentSensor::UpdateBeliefsFromSensors()
{
	for (AActor* Actor : DetectedActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Scanning..."))
		if (Actor->IsA(UThiefAgent::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Saw thief..."))
			ThiefPosition = Actor->GetActorLocation();
			UpdateBelief("ThiefVisible", true);
			WorldState->SetState("ThiefVisible", true);
			UpdateBelief("ThiefVisible", ThiefPosition);
			UpdateBelief("ThiefLastKnownLocation", true);
			GetTheOwner()->GetBeliefs()->SetThiefCurrentLocation(ThiefPosition);
			GetTheOwner()->GetBeliefs()->SetThiefVisible(true, ThiefPosition);
			GetTheOwner()->GetBeliefs()->SetThiefsLastKnownLocation(ThiefPosition);
			GetTheOwner()->TargetActor = Actor;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Lost sight of thief..."))
			ThiefPosition = FVector::ZeroVector;
			WorldState->SetState("ThiefVisible", false);
			UpdateBelief("ThiefVisible", false);
			UpdateBelief("ThiefVisible", FVector::ZeroVector);
			GetTheOwner()->GetBeliefs()->SetThiefCurrentLocation(FVector::ZeroVector);
			GetTheOwner()->GetBeliefs()->SetThiefVisible(false, FVector::ZeroVector);
			GetTheOwner()->TargetActor = nullptr;
		}
		
	} 
}

UPatrolAgent* UPatrolAgentSensor::GetTheOwner()
{
	return Cast<UPatrolAgent>(OwnerAgent);
}

void UPatrolAgentSensor::SetDetectionRange(float range)
{
	DetectionRange = range;
}
