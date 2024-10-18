// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveToWaypointAction.h"  
#include "AIController.h"
#include "PatrolAgent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

class UCharacterMovementComponent;

UMoveToWaypointAction::UMoveToWaypointAction()
{
	cost = 1.0f; 
	//Preconditions.Add("HasWaypoint", true);  
	//Preconditions.Add("AtWaypoint", false);   
	Preconditions.Add("ThiefVisible", false);  
	//PreconditionsVectors.Add("ThiefVisible", FVector::Zero());  
	//PreconditionsVectors.Add("ThiefLastKnownPosition", FVector::Zero());  
	//TODO: Create Patrol nodes and add them to an array from which the agent can patrol around
	
}

bool UMoveToWaypointAction::IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)
{
	const UPatrolAgentBeliefs* DerivedBeliefs = Cast<const UPatrolAgentBeliefs>(&Beliefs); 
	return DerivedBeliefs->IsThiefVisible() == false; 
}

void UMoveToWaypointAction::ApplyEffects(UWorldState& WorldState)
{
	Super::ApplyEffects(WorldState);
	if (UPatrolAgent* Agent = Cast<UPatrolAgent>(GetOuter()))
	{
		Agent->UpdateBelief("AtWaypoint", true);
	}
}

bool UMoveToWaypointAction::IsActionComplete() const
{
	UPatrolAgent* Agent = Cast<UPatrolAgent>(GetOuter());
	if (Agent)
	{
		const FVector NextPosition = Agent->CurrentWayPoint->GetActorLocation();
		if(!NextPosition.IsZero())
		{
			const float distance = FVector::Dist(Agent->GetOwner()->GetActorLocation(), NextPosition);
			if(distance <= 200.0f)
			{
				Agent->GetNextWaypoint();
				return true;
			}
		}
	}
	return false;
}

void UMoveToWaypointAction::PerformAction()
{
	if (UPatrolAgent* Agent = Cast<UPatrolAgent>(GetOuter()))
	{
		Agent->UpdateBelief("AtWaypoint", false);
		Character = Cast<ACharacter>(Agent->GetOwner());
		UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
		MovementComp->MaxWalkSpeed = Agent->GetNormalSpeed();
		Agent->GetAaiController()->MoveToActor(Agent->CurrentWayPoint , 100.0f);
	}
}
