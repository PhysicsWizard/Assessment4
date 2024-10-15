// Fill out your copyright notice in the Description page of Project Settings.


#include "RunAwayAction.h"

#include "ThiefAgent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

URunAwayAction::URunAwayAction()
{
	Preconditions.Add("GoldTaken", true);
	Preconditions.Add("ThiefVisible", false);
	Preconditions.Add("Escaped", false);
	Effects.Add("Escaped", true);
}

bool URunAwayAction::IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)
{
	return Super::IsActionPossible(WorldState,Beliefs );
}


void URunAwayAction::PerformAction()
{ 
	if (UThiefAgent* Agent = Cast<UThiefAgent>(GetOuter()))
	{
		Character = Cast<ACharacter>(Agent->GetOwner());
		UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
		MovementComp->MaxWalkSpeed = Agent->GetRunSpeed();
		FVector ExitVectorLocation(1490.0f, -250.0f, 460.0f);
		Agent->MoveToLocation(ExitVectorLocation);
	}
}

bool URunAwayAction::IsActionComplete() const
{ 
	if (UThiefAgent* Agent = Cast<UThiefAgent>(GetOuter()))
	{
		FVector ExitVectorLocation(1490.0f, -250.0f, 460.0f);
		FVector ThiefLocation = Agent->GetOwner()->GetActorLocation();
		return FVector::Dist(ExitVectorLocation, ThiefLocation) <= 100.0f;
	}
	return false;
}

void URunAwayAction::ApplyEffects(UWorldState& WorldState)
{
	Super::ApplyEffects(WorldState);
}
