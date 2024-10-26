// Fill out your copyright notice in the Description page of Project Settings.

#include "AGP/GoalActionOrientatedPlanning/EnemyAgent.h"
#include "AttackAction.h"
#include "EliminateEnemyGoal.h"
#include "FlankAction.h"
#include "HealAction.h"
#include "StayAliveGoal.h"
#include "SuppressPlayerAction.h"
#include "TakeCoverAction.h"
#include "AGP/Characters/EnemyCharacter.h"
#include "AGP/Characters/HealthComponent.h"


void UEnemyAgent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ManageBeliefsAndPerceptions();
	GetBeliefs()->UpdateBeliefs();
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
}

void UEnemyAgent::BeginPlay()
{
	Super::BeginPlay();
	Goals.Empty();
	Goals.Add(NewObject<UEliminateEnemyGoal>(this));
	Goals.Add(NewObject<UStayAliveGoal>(this));
	AvailableAction.Empty();
	AvailableAction.Add(NewObject<UFlankAction>(this));
	AvailableAction.Add(NewObject<UHealAction>(this));
	AvailableAction.Add(NewObject<USuppressPlayerAction>(this));
	AvailableAction.Add(NewObject<UTakeCoverAction>(this));
	AvailableAction.Add(NewObject<UAttackAction>(this));
	SetUpPerception();
	Beliefs = NewObject<UEnemyAgentBeliefs>(this);
	if(EnemyCharacterComponent)
	{
		HealthComponent = EnemyCharacterComponent->GiveHealthComponent();
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

void UEnemyAgent::ManageBeliefsAndPerceptions()
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

void UEnemyAgent::SetTheOwener(AEnemyCharacter* EnemyCharacter)
{
	EnemyCharacterComponent = EnemyCharacter;
}

UHealthComponent* UEnemyAgent::GetHealthComponent()
{
	return HealthComponent;
}


void UEnemyAgent::SetUpPerception()
{
	/*
	Sensor = NewObject<UPatrolAgentSensor>(GetOwner(), UPatrolAgentSensor::StaticClass(), TEXT("Sensor"));
	Sensor->SetOwnerAgent(this);
	Sensor->RegisterComponent();
	if(!Sensor)
		UE_LOG(LogTemp, Warning, TEXT("No sensor")); 
	if (UPatrolAgentSensor* PatrolSensor = Cast<UPatrolAgentSensor>(Sensor))
	{
		PatrolSensor->SetDetectionRange(500.0f); // Now you can call SetDetectionRange
	}

	PerceptionComponent = NewObject<UAIPerceptionComponent>(GetOwner(), UAIPerceptionComponent::StaticClass(), TEXT("PerceptionComponent"));
	PerceptionComponent->RegisterComponent();
	SightConfig = NewObject<UAISenseConfig_Sight>(this, TEXT("SightConfig"));
	SightConfig->SightRadius = DetectionRange;
	SightConfig->LoseSightRadius = 1000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 110.0f;
	SightConfig->SetMaxAge(5.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &UPatrolAgent::OnPerceptionUpdated);
	StimuliSourceComponent = NewObject<UAIPerceptionStimuliSourceComponent>(GetOwner(), UAIPerceptionStimuliSourceComponent::StaticClass(), TEXT("StimuliSourceComponent"));
	StimuliSourceComponent->RegisterComponent();
	StimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
	StimuliSourceComponent->RegisterWithPerceptionSystem();
	*/
}
