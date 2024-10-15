// Fill out your copyright notice in the Description page of Project Settings.


#include "StealAction.h"

#include "AIController.h"
#include "ThiefAgent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

class UCharacterMovementComponent;

UStealAction::UStealAction()
{
	cost = 10;
	Preconditions.Add("GoldTaken", false);
	Preconditions.Add("ThiefVisible", false);
	Effects.Add("GoldTaken", true);
}

bool UStealAction::IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)
{
	return WorldState.GetWorldState()["GoldTaken"] == false && WorldState.GetWorldState()["ThiefVisible"] == false;
}

void UStealAction::PerformAction()
{ 
	if (UThiefAgent* Agent = Cast<UThiefAgent>(GetOuter()))
	{
		if(Agent->Gold != nullptr)
		{
			const FVector GoldLocation = Agent->GetGoldLocation();
			//UE_LOG(LogTemp, Log, TEXT("Performining steal"))
			Character = Cast<ACharacter>(Agent->GetOwner());
			UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
			MovementComp->MaxWalkSpeed = Agent->GetNormalSpeed();
			Agent->GetAaiController()->MoveToLocation(GoldLocation , -1.0f);
		}
	}
}

bool UStealAction::IsActionComplete() const
{
	UThiefAgent* Agent = Cast<UThiefAgent>(GetOuter());
	AActor* actor = Agent->GetOwner();
	float distance = FVector::Dist(actor->GetActorLocation(),Agent->GetGoldLocation());
	return distance <= 250.0f;
}

void UStealAction::ApplyEffects(UWorldState& WorldState)
{
	Super::ApplyEffects(WorldState);
}
