// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentSensor.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h" 
#include "Agent.h"

class UAgent;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;

// Sets default values for this component's properties
UAgentSensor::UAgentSensor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DetectionRange = 500.0f;
	PersistantSightRange = 750.0f;
	FieldOfView = 70.0f;
	OwnerAgent = nullptr;

	/*
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    
	// Set the sight parameters
	SightConfig->SightRadius = 500.0f;             // Set the sight radius
	SightConfig->LoseSightRadius = 750.0f;        // Set the lose sight radius
	SightConfig->PeripheralVisionAngleDegrees = FieldOfView; // Set the peripheral vision angle
    
	// Add the sight sense to the perception component
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &UAgentSensor::OnPerceptionUpdated);
	*/

}


void UAgentSensor::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	DetectedActors = UpdatedActors;

	//TODO: Implement detection logic here

	UpdateBeliefsFromSensors();
}

TArray<AActor*> UAgentSensor::GetDetectedActors() const
{
	return DetectedActors;
}

void UAgentSensor::UpdateBeliefsFromSensors()
{
}
 
// Called when the game starts
void UAgentSensor::BeginPlay()
{
	Super::BeginPlay();
	WorldState = UWorldState::GetInstance(); 

	// ...
	
}


// Called every frame
void UAgentSensor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction); 
	// ...
	
}


void UAgentSensor::SetOwnerAgent(UAgent* Agent)
{
	OwnerAgent = Agent;
}

UAgent* UAgentSensor::GetTheOwner()
{
	return OwnerAgent;
}

void UAgentSensor::UpdateBelief(const FString& Key, bool Value)
{
	if(OwnerAgent)
		OwnerAgent->UpdateBelief(Key, Value);
}

void UAgentSensor::UpdateBelief(const FString& Key, FVector Value)
{
	if(OwnerAgent)
		OwnerAgent->UpdateBelief(Key, Value);
}


