// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaseThiefAction.h"
 
#include "GameFramework/Character.h"  // Include ACharacter
#include "GameFramework/CharacterMovementComponent.h"   
#include "AIController.h"
#include "PatrolAgent.h"
#include "PatrolAgentBeliefs.h"
#include "PatrolGoal.h"

UChaseThiefAction::UChaseThiefAction()
{
	cost = 2.0f;  // Set a higher cost for chasing
	Preconditions.Add("ThiefVisible", true);  // The thief must be visible to chase 
	//Effects.Add("ThiefVisible", false);  // After the action, the thief should no longer be visible 
	//Effects.Add("ThiefLastKnownLocation", true);  // After the action, the thief should no longer be visible
	//EffectVectors.Add("ThiefVisible", FVector::Zero());  // After the action, the thief should no longer be visible
	//EffectVectors.Add("ThiefLastKnownLocation", FVector::Zero());  
}

bool UChaseThiefAction::IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)
{ 
	return false;
}

void UChaseThiefAction::PerformAction()
{
	if (UPatrolAgent* Agent = Cast<UPatrolAgent>(GetOuter()))
	{
		Character = Cast<ACharacter>(Agent->GetOwner());
		UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
		//Dosen't work for some reason...
		//MovementComp->MaxWalkSpeed =  Agent->GetRunSpeed();
		MovementComp->MaxWalkSpeed =  250.0f;
		Agent->GetAaiController()->SetFocus(Agent->TargetActor);
		Agent->GetAaiController()->MoveToActor(Agent->TargetActor , 50.0f);
	} 
}

bool UChaseThiefAction::IsActionComplete() const
{
	if (UPatrolAgent* Agent = Cast<UPatrolAgent>(GetOuter()))
	{
		UWorldState* WorldState = Agent->GetWorldState();
		FVector enemyPos = WorldState->GetWorldVectorState()["PatrolAgentPosition"];
		FVector thiefPos = WorldState->GetWorldVectorState()["ThiefAgentLocation"];
		if(FVector::Dist(enemyPos, thiefPos)<=50.0f){
			WorldState->SetState("GoldTaken", false); // during the process of chasing the thief, if they get close, take the gold back lol
			}
		return !Agent->GetBeliefs()->IsThiefVisible();
	}
	return false;
}

void UChaseThiefAction::ApplyEffects(UWorldState& WorldState)
{
	Super::ApplyEffects(WorldState);
}
