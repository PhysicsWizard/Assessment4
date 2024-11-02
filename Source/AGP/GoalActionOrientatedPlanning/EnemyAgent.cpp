// Fill out your copyright notice in the Description page of Project Settings.

#include "AGP/GoalActionOrientatedPlanning/EnemyAgent.h"
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

	if (CurrentGoal)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Goal: %s"), *CurrentGoal->GetName());
	} 
	if (CurrentPlan.Num() > 0 && CurrentPlan[0])
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Action: %s"), *CurrentPlan[0]->GetName());
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
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay in UEnemyAgent called"));
	Goals.Empty();
	Goals.Add(NewObject<UEliminateEnemyGoal>(this));
	Goals.Add(NewObject<UStayAliveGoal>(this));
	UE_LOG(LogTemp, Warning, TEXT("Goals count: %d"), Goals.Num()); 
	AvailableAction.Empty();
	AvailableAction.Add(NewObject<UPatrolAction>(this));
	AvailableAction.Add(NewObject<UHealAction>(this));
	AvailableAction.Add(NewObject<UAttackAction>(this));
	AvailableAction.Add(NewObject<UChargeAttackAction>(this));
	AvailableAction.Add(NewObject<URetreatAction>(this));
	UE_LOG(LogTemp, Warning, TEXT("AvailableAction count: %d"), AvailableAction.Num()); 
	/*
	 * Possible do these three not sure yet
	 *AvailableAction.Add(NewObject<UFlankAction>(this));
	AvailableAction.Add(NewObject<USuppressPlayerAction>(this));
	AvailableAction.Add(NewObject<UTakeCoverAction>(this));
	*/
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
	if(EnemyCharacterComponent)
	{ 
		if(const APlayerCharacter* Character = EnemyCharacterComponent->GetSensedCharacter())
		{
			if(FVector::Dist(EnemyCharacterComponent->GetActorLocation(), Character->GetActorLocation()) > 400.0f)
			{
				GetBeliefs()->GetBeliefsState()["WithinRange"] = false;
			}
			else
			{
				GetBeliefs()->GetBeliefsState()["WithinRange"] = true;
			}
			GetBeliefs()->GetBeliefsState()["TargetSpotted"] = true;
			GetBeliefs()->GetBeliefsStateVectors()["TargetPosition"] = Character->GetActorLocation();
			GetBeliefs()->GetBeliefsStateVectors()["LastKnownTargetPosition"] = Character->GetActorLocation();
			UE_LOG(LogTemp, Log, TEXT("TargetPosition: %s"), *Character->GetActorLocation().ToString());
			UE_LOG(LogTemp, Log, TEXT("LastKnownTargetPosition: %s"), *Character->GetActorLocation().ToString());
		}
		else
		{
			GetBeliefs()->GetBeliefsState()["TargetSpotted"] = false;
			GetBeliefs()->GetBeliefsStateVectors()["TargetPosition"] = FVector::ZeroVector;
		}
		PlanActions();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Character Component Invalid is null"));
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


