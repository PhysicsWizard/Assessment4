// Fill out your copyright notice in the Description page of Project Settings.

#include "AGP/GoalActionOrientatedPlanning/EnemyAgent.h"

#include "AdvanceAction.h"
#include "AttackAction.h"
#include "ChargeAttackAction.h"
#include "EliminateEnemyGoal.h"
#include "FlankAction.h"
#include "HealAction.h"
#include "PatrolAction.h"
#include "RetreatAction.h"
#include "StayAliveGoal.h"
#include "SuppressPlayerAction.h"
#include "TakeCoverAction.h"
#include "AGP/Characters/EnemyCharacter.h"
#include "AGP/Characters/HealthComponent.h"


void UEnemyAgent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{ 
	ManageHealthBeliefs();
	ManageSensedCharacters();
	GetBeliefs()->UpdateBeliefs();

	UE_LOG(LogTemp, Warning, TEXT("Beliefs - TargetSpotted: %s, WithinRange: %s"),
	   GetBeliefs()->GetBeliefsState()["TargetSpotted"] ? TEXT("true") : TEXT("false"),
	   GetBeliefs()->GetBeliefsState()["WithinRange"] ? TEXT("true") : TEXT("false"));

	
	if (CurrentGoal)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Goal: %s"), *CurrentGoal->GetName());
	}
	
	if (CurrentPlan.Num() > 0 && CurrentPlan[0])
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Action: %s"), *CurrentPlan[0]->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Current Plan length: %d"), CurrentPlan.Num());
	} 
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UEnemyAgent::PerformAction()
{
	Super::PerformAction();
}

bool UEnemyAgent::PlanActions()
{
	return Super::PlanActions();
}

UEnemyAgent::UEnemyAgent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyAgent::BeginPlay()
{
	Super::BeginPlay();
	Goals.Empty();
	Goals.Add(NewObject<UEliminateEnemyGoal>(this));
	Goals.Add(NewObject<UStayAliveGoal>(this)); 
	AvailableAction.Empty();
	AvailableAction.Add(NewObject<UChargeAttackAction>(this));
	AvailableAction.Add(NewObject<UAttackAction>(this));
	AvailableAction.Add(NewObject<UAdvanceAction>(this));
	AvailableAction.Add(NewObject<UPatrolAction>(this));
	AvailableAction.Add(NewObject<UHealAction>(this));
	AvailableAction.Add(NewObject<URetreatAction>(this));  
	Beliefs = NewObject<UEnemyAgentBeliefs>(this);
	WorldState = UWorldState::GetInstance();
	EnemyCharacterComponent = Cast<AEnemyCharacter>(GetOuter());
	if(EnemyCharacterComponent)
	{
		HealthComponent = EnemyCharacterComponent->GiveHealthComponent();
		if(!HealthComponent)
		{
			HealthComponent = EnemyCharacterComponent->FindComponentByClass<UHealthComponent>();
			if(!HealthComponent)
			{
				UE_LOG(LogTemp, Error, TEXT("No health component found!!!")); 
			}
		}
	}

	if (!IsActive())
	{
		SetActive(true);
	}
}

UEnemyAgentBeliefs* UEnemyAgent::GetBeliefs() const
{
	return Cast<UEnemyAgentBeliefs>(Beliefs);
}
UEnemyAgentBeliefs* UEnemyAgent::GetBeliefs()
{
	return Cast<UEnemyAgentBeliefs>(Beliefs);
}
AEnemyCharacter* UEnemyAgent::GetEnemyCharacterComponent()
{
	return EnemyCharacterComponent;
}

void UEnemyAgent::ManageHealthBeliefs()
{
	//Updating health beliefs
	GetBeliefs()->SetCurrentHealthPercentage(HealthComponent->GetCurrentHealthPercentage());
}

void UEnemyAgent::ManageSensedCharacters()
{
	if (EnemyCharacterComponent)
	{ 
		if (const APlayerCharacter* Character = EnemyCharacterComponent->GetSensedCharacter())
		{
			const float Distance = FVector::Dist(EnemyCharacterComponent->GetActorLocation(), Character->GetActorLocation());
			UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), Distance);
			const bool bWithinRange = Distance <= 10000.0f;

			// Now we're modifying the actual belief map
			Beliefs->GetBeliefsState()["WithinRange"] = bWithinRange;
			Beliefs->GetBeliefsState()["TargetSpotted"] = true;
			Beliefs->GetBeliefsStateVectors()["TargetPosition"] = Character->GetActorLocation();
			Beliefs->GetBeliefsStateVectors()["LastKnownTargetPosition"] = Character->GetActorLocation();
		}
		else
		{
			Beliefs->GetBeliefsState()["TargetSpotted"] = false;
			Beliefs->GetBeliefsState()["WithinRange"] = false;
			Beliefs->GetBeliefsStateVectors()["TargetPosition"] = FVector::ZeroVector;
		}
		PlanActions();
	}
}


void UEnemyAgent::SetTheOwner(AEnemyCharacter* EnemyCharacter)
{
	EnemyCharacterComponent = EnemyCharacter;
}

UHealthComponent* UEnemyAgent::GetHealthComponent()
{
	return HealthComponent;
}

FEnemyStats* UEnemyAgent::GetEnemyStats()
{
	return GetEnemyCharacterComponent()->GetStats();
}


