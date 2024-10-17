// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolAgent.h"  
#include "AIController.h"
#include "ChaseThief.h"
#include "ChaseThiefAction.h"
#include "InvestigateAreaAction.h"
#include "InvestigateGoal.h"
#include "MoveToWaypointAction.h"
#include "PatrolAgentBeliefs.h"
#include "PatrolAgentSensor.h"
#include "PatrolGoal.h"
#include "ThiefAgent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

UPatrolAgent::UPatrolAgent()
{
	currentIndex = 0;
	RunSpeed = 30.0f;
	NormalSpeed = 15.0f;
	DetectionRange = 500.0f;
	
}

void UPatrolAgent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(WorldState)
		WorldState->SetVectorState("PatrolAgentPosition", GetOwner()->GetActorLocation());

	if(TargetActor) 
		GetAaiController()->SetFocus(TargetActor);  
	else
		GetAaiController()->ClearFocus(EAIFocusPriority::Gameplay); 
	
	bDetermineLineOfSight();
	PerformDetection();

	const bool bIsVisible = WorldState->GetWorldState()["ThiefVisible"];
	//UE_LOG(LogTemp, Warning, TEXT("IsVisible in Patrol: %s"), bIsVisible ? TEXT("true") : TEXT("false"));

}

bool UPatrolAgent::PlanActions()
{
	return Super::PlanActions();
}

void UPatrolAgent::PerformAction()
{
	Super::PerformAction();
}

AActor* UPatrolAgent::GetNextWaypoint()
{
	if (Waypoints.Num() > 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Waypoints.Num(): %d, currentIndex: %d before update"), Waypoints.Num(), currentIndex);

		currentIndex = (currentIndex + 1) % Waypoints.Num();
		//UE_LOG(LogTemp, Warning, TEXT("New currentIndex: %d after update"), currentIndex);

		return CurrentWayPoint = Waypoints[currentIndex];
	}
	//UE_LOG(LogTemp, Warning, TEXT("No waypoints available"));
	return nullptr;
}

void UPatrolAgent::MoveToLocation(FVector& location)
{
	AIController->MoveToLocation(location);
}

void UPatrolAgent::BeginPlay()
{
	Super::BeginPlay();
	Goals.Empty();
	Goals.Add(NewObject<UInvestigateGoal>(this));
	Goals.Add(NewObject<UChaseThief>(this));
	Goals.Add(NewObject<UPatrolGoal>(this));
	AvailableAction.Empty();
	AvailableAction.Add(NewObject<UInvestigateAreaAction>(this));
	AvailableAction.Add(NewObject<UMoveToWaypointAction>(this));
	AvailableAction.Add(NewObject<UChaseThiefAction>(this));
	
	// Initialize WorldState as a singleton
	WorldState = UWorldState::GetInstance();
	if(!WorldState)
		UE_LOG(LogTemp, Log, TEXT("WorldState is null"));
	Beliefs = NewObject<UPatrolAgentBeliefs>(this);  // Ensure it’s attached to the actor
	if(!Beliefs)
		UE_LOG(LogTemp, Log, TEXT("Beliefs is null")); 
	
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
	
	// Set the initial waypoint
	if (Waypoints.Num() > 0)
	{
		CurrentWayPoint = Waypoints[currentIndex];
	}
}

UPatrolAgentBeliefs* UPatrolAgent::GetBeliefs() const
{
	return Cast<UPatrolAgentBeliefs>(Beliefs);
}

UPatrolAgentBeliefs* UPatrolAgent::GetBeliefs()
{
	return Cast<UPatrolAgentBeliefs>(Beliefs);
}

void UPatrolAgent::PerformDetection()
{
	    AActor* Owner = GetOwner();
    if (!Owner)
        return;

    FVector StartLocation = Owner->GetActorLocation();
    FVector ForwardVector = Owner->GetActorForwardVector();

    float FOVAngle = 120.0f;        // Total field of view in degrees
    int32 NumberOfRays = 120;         // Number of line traces

    // Calculate the angle increment between each ray
    float AngleIncrement = FOVAngle / (NumberOfRays - 1);

    bool bThiefVisible = false;
    FVector ThiefPosition = FVector::ZeroVector;

    for (int32 i = 0; i < NumberOfRays; ++i)
    {
	    float DetectRang = 1000.0f;
	    // Correctly calculate the angle for this ray
        float AngleDegrees = -FOVAngle / 2 + i * AngleIncrement;

        // Calculate the direction vector for this ray
        FVector Direction = ForwardVector.RotateAngleAxis(AngleDegrees, FVector::UpVector);

        FVector EndLocation = StartLocation + (Direction * DetectRang);

        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(Owner);

        // Perform the line trace
        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

        // Debug line
         

        if (bHit && HitResult.GetActor() != nullptr)
        { 
            AActor* HitActor = HitResult.GetActor();
            if (HitActor->FindComponentByClass<UThiefAgent>())
            {
            	TargetActor = HitActor;
            	TargetActorCopy = HitActor;
            	//UE_LOG(LogTemp, Log, TEXT("See Thief..."));
                bThiefVisible = true;
                break; // Exit the loop if a patrol agent is detected
            }
        }
    }
	if(bThiefVisible)
	{
		DrawDebugLine(GetWorld(), StartLocation, TargetActor->GetActorLocation(), FColor::Blue, false, 1.0f);
		ThiefPosition = TargetActor->GetActorLocation(); 

		GetBeliefs()->SetThiefCurrentLocation(ThiefPosition);
		UpdateBelief("ThiefVisible", true);
		//WorldState->SetState("ThiefVisible", true);
		UpdateBelief("ThiefVisible", ThiefPosition);
		UpdateBelief("ThiefLastKnownPosition", ThiefPosition);
		GetBeliefs()->SetThiefCurrentLocation(ThiefPosition);
		GetBeliefs()->SetThiefVisible(true, ThiefPosition);
		GetBeliefs()->SetThiefsLastKnownLocation(ThiefPosition);
		GetAaiController()->SetFocus(TargetActor);
		PlanActions();
	}
    else if (!bThiefVisible)
    {
    	//UE_LOG(LogTemp, Log, TEXT("Lost sight of Thief..."));
    	UpdateBelief("ThiefVisible", false);
    	WorldState->SetState("ThiefVisible", false);
    	UpdateBelief("ThiefVisible", FVector::ZeroVector);
    	GetBeliefs()->SetThiefCurrentLocation(FVector::ZeroVector);
    	GetBeliefs()->SetThiefVisible(false, FVector::ZeroVector);
    	TargetActor = nullptr;
    	GetAaiController()->ClearFocus(EAIFocusPriority::Gameplay);
    	PlanActions();
    }
}

bool UPatrolAgent::inLineOfSight()
{
	AActor* Owner = GetOwner();
	FVector StartLocation = Owner->GetActorLocation();
	FVector EndLocation = TargetActorCopy->GetActorLocation();
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Owner);
	
	bool bLineOfSight = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);
	return bLineOfSight;
}

void UPatrolAgent::bDetermineLineOfSight()
{
	AActor* Owner = GetOwner();
	FVector StartLocation = Owner->GetActorLocation();
	if(!TargetActor)
		return;
	
	if(inLineOfSight())
	{
		FVector ThiefPosition = TargetActor->GetActorLocation();
		DrawDebugLine(GetWorld(), StartLocation, TargetActor->GetActorLocation(), FColor::Blue, false, 1.0f);
		ThiefPosition = TargetActor->GetActorLocation(); 

		GetBeliefs()->SetThiefCurrentLocation(ThiefPosition);
		UpdateBelief("ThiefVisible", true);
		WorldState->SetState("ThiefVisible", true);
		UpdateBelief("ThiefVisible", ThiefPosition);
		UpdateBelief("ThiefLastKnownPosition", ThiefPosition);
		GetBeliefs()->SetThiefCurrentLocation(ThiefPosition);
		GetBeliefs()->SetThiefVisible(true, ThiefPosition);
		GetBeliefs()->SetThiefsLastKnownLocation(ThiefPosition);
		GetAaiController()->SetFocus(TargetActor);
		PlanActions();
	}
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("Lost sight of Thief..."));
		UpdateBelief("ThiefVisible", false);
		UpdateBelief("ThiefVisible", FVector::ZeroVector);
		GetBeliefs()->SetThiefCurrentLocation(FVector::ZeroVector);
		GetBeliefs()->SetThiefVisible(false, FVector::ZeroVector);
		TargetActor = nullptr;
		GetAaiController()->ClearFocus(EAIFocusPriority::Gameplay);
		PlanActions();
	}
}

void UPatrolAgent::OnPerceptionUpdated(const TArray<AActor*>& Actors){
	UE_LOG(LogTemp, Warning, TEXT("Actors counted: %d "), Actors.Num());
	bool bThiefVisible = false;
	FVector ThiefPosition = FVector::ZeroVector;
	AActor* actor = nullptr;
	for (AActor* Actor : Actors)
	{
		if (Actor->FindComponentByClass<UThiefAgent>())
		{
			UE_LOG(LogTemp, Warning, TEXT("Saw agent..."));
			ThiefPosition = Actor->GetActorLocation();
			bThiefVisible = true;
			actor = Actor;
			break; // Exit the loop since we've found the agent
		}
	}
	if (bThiefVisible)
	{
		GetBeliefs()->SetThiefCurrentLocation(ThiefPosition);
		UpdateBelief("GuardVisible", true);
		WorldState->SetState("ThiefVisible", true);
		UpdateBelief("GuardPosition", ThiefPosition);
		UpdateBelief("GuardLastKnownPosition", ThiefPosition);
		GetBeliefs()->SetThiefCurrentLocation(ThiefPosition);
		GetBeliefs()->SetThiefVisible(true, ThiefPosition);
		GetBeliefs()->SetThiefsLastKnownLocation(ThiefPosition);
		TargetActor = actor;

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Lost sight of agent..."));
		UpdateBelief("GuardVisible", false);
		WorldState->SetState("ThiefVisible", false);
		UpdateBelief("GuardPosition", FVector::ZeroVector);
		GetBeliefs()->SetThiefCurrentLocation(FVector::ZeroVector);
		GetBeliefs()->SetThiefVisible(false, FVector::ZeroVector);
		TargetActor = nullptr;
	}
}


