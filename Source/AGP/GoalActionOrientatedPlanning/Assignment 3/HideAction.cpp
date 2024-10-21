// Fill out your copyright notice in the Description page of Project Settings.


#include "HideAction.h"

#include "AIController.h"
#include "PatrolAgent.h"
#include "ThiefAgent.h"
#include "ThiefAgentBeliefs.h"
#include "ThiefAgentSesnor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

class UThiefAgent;

UHideAction::UHideAction()
{
	cost = 1.0f;  
	Preconditions.Add("ThiefVisible", true);  
	Effects.Add("ThiefVisible", false);  
	//EffectVectors.Add("ThiefVisible", FVector::Zero());  
}

bool UHideAction::IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)
{
	return WorldState.GetWorldState()["ThiefVisible"] == true;
}

void UHideAction::PerformAction()
{
	if(UThiefAgent* Agent = Cast<UThiefAgent>(GetOuter()))
	{
		FVector HidePoint;
		Character = Cast<ACharacter>(Agent->GetOwner());
		UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
		MovementComp->MaxWalkSpeed =  350.0f;
		if(!Agent->WayPoint)
			HidePoint = Agent->FurthestPoint();
		else
			HidePoint = Agent->WayPoint->GetActorLocation();
		Agent->GetAaiController()->MoveToLocation(HidePoint);
	} 
	
}

bool UHideAction::IsActionComplete() const
{
	if(UThiefAgent* Agent = Cast<UThiefAgent>(GetOuter()))
	{
		/*
		if (Agent->InLOS() == false)
		{
			Agent->GetWorldState()->SetState("ThiefVisible", false);
			return true;
		}
		*/
		FVector enemyPos = Agent->GetWorldState()->GetWorldVectorState()["PatrolAgentPosition"];
		FVector thiefPos = Agent->GetWorldState()->GetWorldVectorState()["ThiefAgentLocation"];
		//UE_LOG(LogTemp, Warning, TEXT("Enemy Position: %s, Thief Position: %s, Distance: %f"), 
      // *enemyPos.ToString(), *thiefPos.ToString(), FVector::Dist(thiefPos, enemyPos));

		if ( FVector::Dist(thiefPos, enemyPos) <= 50.0f)
		{
			Agent->GetWorldState()->SetState("ThiefVisible", false);
			Agent->WayPoint = nullptr;
			return true;
		}
	} 
	return false;
}

void UHideAction::ApplyEffects(UWorldState& WorldState)
{
	Super::ApplyEffects(WorldState);
}
