// Fill out your copyright notice in the Description page of Project Settings.


#include "InvestigateAreaAction.h"
#include "NavigationSystem.h"
#include "Agent.h"
#include "PatrolAgent.h"
#include "PatrolAgentBeliefs.h"
#include "PatrolGoal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

class UCharacterMovementComponent;

UInvestigateAreaAction::UInvestigateAreaAction()
{
	cost = 1.5;
	Preconditions.Add("ThiefVisible", false);
	Preconditions.Add("ThiefLastKnownLocation", true);
	EffectVectors.Add("ThiefVisible", FVector::Zero());
	Effects.Add("ThiefLastKnownLocation", false);
	RandomPosition = FVector::Zero();
}

bool UInvestigateAreaAction::IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)
{
	const UPatrolAgentBeliefs* DerivedBeliefs = Cast<const UPatrolAgentBeliefs>(&Beliefs);
	return !DerivedBeliefs->GetThiefsLastKnownLocation().IsZero() && DerivedBeliefs->IsThiefVisible() == false;
}

FVector UInvestigateAreaAction::GetRandomPoint()
{
	AActor* OwnerActor = Cast<UPatrolAgent>(GetOuter())->GetOwner();
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(OwnerActor->GetWorld());
	FVector Origin = OwnerActor->GetActorLocation();
	FVector RandomPoint;
	FNavLocation NavLocation;
	
	bool bFoundLocation = NavSystem->GetRandomReachablePointInRadius(Origin, 1000.0f, NavLocation);
	if (bFoundLocation)
		RandomPoint = NavLocation.Location;
	
	else
		RandomPoint= FVector::Zero();
	
	return RandomPoint;
}

void UInvestigateAreaAction::PerformAction()
{
	if (UPatrolAgent* Agent = Cast<UPatrolAgent>(GetOuter()))
	{
		Character = Cast<ACharacter>(Agent->GetOwner());
		UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
		MovementComp->MaxWalkSpeed = Agent->GetRunSpeed();
		AActor* OwnerActor = Agent->GetOwner();
		UPatrolAgentBeliefs* PatrolBeliefs = Cast<UPatrolAgentBeliefs>(Agent->GetBeliefs());
		if (PatrolBeliefs)
		{
			if (!bInvestigateArea)
			{
				// Move to the thief's last known location
				FVector Location = PatrolBeliefs->GetThiefsLastKnownLocation();
				Agent->MoveToLocation(Location);

				// Check if the patrol agent has arrived at the thief's last known location
				if (FVector::Dist(OwnerActor->GetActorLocation(), Location) <= 50.0f)
				{
					bInvestigateArea = true;
					RandomPosition = GetRandomPoint();  // Get a random point to investigate
				}
			}
            
			// If the agent has already investigated the last known location, move to the random point
			if (bInvestigateArea && RandomPosition != FVector::Zero())
			{
				Agent->MoveToLocation(RandomPosition);

				if (FVector::Dist(OwnerActor->GetActorLocation(), RandomPosition) <= 50.0f)
				{
					bInvestigationDone = true;
				}
			}
		}

		if(PatrolBeliefs->IsThiefVisible())
		{
			Effects.Add("ThiefVisible", true);
		}
	}
}

bool UInvestigateAreaAction::IsActionComplete() const
{
	return bInvestigationDone;
		
}

void UInvestigateAreaAction::ApplyEffects(UWorldState& WorldState)
{
	Super::ApplyEffects(WorldState);
	WorldState.SetVectorState("ThiefLastKnownLocation", FVector::ZeroVector);
}
