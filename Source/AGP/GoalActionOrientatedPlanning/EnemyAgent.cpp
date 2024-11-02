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
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ManageHealthBeliefs();
	ManageSensedCharacters();
	GetBeliefs()->UpdateBeliefs();
	
	// Print checks
	if (CurrentGoal)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Goal: %s"), *CurrentGoal->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Goal is null"));
	}
	UE_LOG(LogTemp, Warning, TEXT("CurrentPlan length: %d"), CurrentPlan.Num());
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
	if(!EnemyCharacterComponent)
	{
		EnemyCharacterComponent = Cast<AEnemyCharacter>(GetOwner());
		if(EnemyCharacterComponent)
		{
			EnemyCharacterComponent = Cast<AEnemyCharacter>(GetOuter());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No enemy character component found!!!")); 
		}
	}

	if(EnemyCharacterComponent)
	{
		HealthComponent = EnemyCharacterComponent->GiveHealthComponent();
		if(!HealthComponent)
		{
			HealthComponent = EnemyCharacterComponent->FindComponentByClass<UHealthComponent>();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No health component found!!!")); 
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

	//Updating the position of a target
	if(EnemyCharacterComponent->GetSensedCharacter()!=nullptr)
	{
		TPair<FString, FVector> TargetMemory(TEXT("LastKnownTargetPosition"), EnemyCharacterComponent->GetSensedCharacter()->GetActorLocation());
		GetBeliefs()->UpdateBeliefsStateVectors(TargetMemory);
		GetBeliefs()->GetBeliefsState()["WithinRange"] = true;
	}
	else
	{
		GetBeliefs()->GetBeliefsState()["WithinRange"] = false;
	}
}

void UEnemyAgent::ManageSensedCharacters()
{
	if(EnemyCharacterComponent)
	{
		if(const ACharacter* Character = EnemyCharacterComponent->GetSensedCharacter())
		{
			if(FVector::Dist(EnemyCharacterComponent->GetActorLocation(), Character->GetActorLocation()) > 300.0f)
			{
				GetBeliefs()->GetBeliefsState()["WithinRange"] = false;
			}
			GetBeliefs()->GetBeliefsState()["WithinRange"] = false;
			GetBeliefs()->GetBeliefsState()["TargetSpotted"] = true;
			GetBeliefs()->GetBeliefsStateVectors()["TargetPosition"] = Character->GetActorLocation();
			GetBeliefs()->GetBeliefsStateVectors()["LastKnownTargetPosition"] = Character->GetActorLocation();
		}
		else
		{
			GetBeliefs()->GetBeliefsState()["TargetSpotted"] = false;
			GetBeliefs()->GetBeliefsStateVectors()["TargetPosition"] = FVector::ZeroVector;
		}
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


