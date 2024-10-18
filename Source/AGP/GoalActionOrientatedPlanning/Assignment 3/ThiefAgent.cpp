// Fill out your copyright notice in the Description page of Project Settings.


#include "ThiefAgent.h"

#include "AIController.h"
#include "EvadeDetectionGoal.h"
#include "HideAction.h"
#include "PatrolAgent.h"
#include "RunAwayAction.h"
#include "RunAwayGoal.h"
#include "StealAction.h"
#include "StealItemGoal.h"
#include "ThiefAgentSesnor.h"

UThiefAgent::UThiefAgent()
{
	RunSpeed = 50.0f;
	NormalSpeed = 10.0f;
	DetectionRange = 500.0f;
}

void UThiefAgent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	WorldState->SetVectorState("ThiefAgentLocation", GetOwner()->GetActorLocation());
	//WorldState->SetState("ThiefVisible", InLOS());
	PerformDetection();

	if(WorldState->GetWorldState()["ThiefVisible"])
		PlanActions();

	
	for (const UAction* Action : CurrentPlan)
	{
		if (Action)
		{
			UE_LOG(LogTemp, Warning, TEXT("Action Class Name: %s"), *Action->GetClass()->GetName());
		}
	}

	const bool bIsVisible = WorldState->GetWorldState()["ThiefVisible"];
	//UE_LOG(LogTemp, Warning, TEXT("IsVisible in thief: %s"), bIsVisible ? TEXT("true") : TEXT("false"));
}

bool UThiefAgent::PlanActions()
{
	return Super::PlanActions();
}

void UThiefAgent::PerformAction()
{
	Super::PerformAction();
}

UThiefAgentBeliefs* UThiefAgent::GetBeliefs() const
{
	return Cast<UThiefAgentBeliefs>(Beliefs);
}

UThiefAgentBeliefs* UThiefAgent::GetBeliefs()
{
	return Cast<UThiefAgentBeliefs>(Beliefs);
}

void UThiefAgent::MoveToLocation(FVector& location)
{
	AIController->MoveToLocation(location);
}

FVector UThiefAgent::GetGoldLocation()
{
	return Gold->GetActorLocation();
}

void UThiefAgent::PerformDetection()
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

    bool bGuardVisible = false;
    FVector GuardPosition = FVector::ZeroVector;

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
        	//UE_LOG(LogTemp, Warning, TEXT("Detected patrol agent at location: %s"), *GuardPosition.ToString());
        	//UE_LOG(LogTemp, Warning, TEXT("Actor Detected"));
            AActor* HitActor = HitResult.GetActor();
            if (HitActor->FindComponentByClass<UPatrolAgent>())
            {
            	DrawDebugLine(GetWorld(), StartLocation, HitActor->GetActorLocation(), FColor::Red, false, 1.0f);
                GuardPosition = HitActor->GetActorLocation(); 
                // Update beliefs
                UpdateBelief("GuardVisible", true);
                UpdateBelief("GuardPosition", GuardPosition);
                UpdateBelief("GuardLastKnownPosition", GuardPosition);
            	PlanActions();
                bGuardVisible = true;
                break; // Exit the loop if a patrol agent is detected
            }
        }
    }

    if (!bGuardVisible)
    {
        //UE_LOG(LogTemp, Warning, TEXT("No patrol agent detected within FOV."));
        // Update beliefs
        UpdateBelief("GuardVisible", false);
        UpdateBelief("GuardPosition", FVector::ZeroVector);
    	PlanActions();
    }
}

bool UThiefAgent::InLOS()
{
	AActor* Owner = GetOwner();
	FVector StartLocation = Owner->GetActorLocation();
	GetWorldState()->GetWorldVectorState()["PatrolAgentPosition"];
	FVector EndLocation = GetWorldState()->GetWorldVectorState()["PatrolAgentPosition"];
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Owner);
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Black, false, 1.0f);
	bool bLineOfSight = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);
	return bLineOfSight; 
}

FVector UThiefAgent::ClosestPoint()
{
	FVector FurthestPoint = FVector::ZeroVector;  // Initial point, can be any point or zero vector
	float closestDistance  = FLT_MAX;  // Start with the smallest possible distance

	FVector StartLocation = GetOwner()->GetActorLocation();
	for (auto point : Waypoints)
	{
		float distance = FVector::Dist(StartLocation, point->GetActorLocation());
		if (distance < closestDistance )
		{
			closestDistance  = distance;
			FurthestPoint = point->GetActorLocation();
			
		}
	}
	return FurthestPoint;
}

FVector UThiefAgent::FurthestPoint()
{
	FVector FurthestPoint = FVector::ZeroVector;  // Initial point, can be any point or zero vector
	float closestDistance  = 0.0f;  // Start with the smallest possible distance

	FVector StartLocation = GetOwner()->GetActorLocation();
	for (auto point : Waypoints)
	{
		float distance = FVector::Dist(StartLocation, point->GetActorLocation());
		if (distance > closestDistance )
		{
			closestDistance  = distance;
			FurthestPoint = point->GetActorLocation();
			WayPoint = point;
		}
	}
	return FurthestPoint;
}


void UThiefAgent::OnPerceptionUpdated(const TArray<AActor*>& Actors)
{ 
	bool bGuardVisible = false;
	FVector GuardPosition = FVector::ZeroVector;

	for (AActor* Actor : Actors)
	{
		if (Actor->FindComponentByClass<UPatrolAgent>())
		{ 
			GuardPosition = Actor->GetActorLocation();
			bGuardVisible = true;
			break; // Exit the loop since we've found the agent
		}
	}
	if (bGuardVisible)
	{
		UpdateBelief("GuardVisible", true);
		UpdateBelief("GuardPosition", GuardPosition);
		UpdateBelief("GuardLastKnownPosition", GuardPosition);
		
	}
	else
	{ 
		UpdateBelief("GuardVisible", false);
		UpdateBelief("GuardPosition", FVector::ZeroVector);
	}
}



void UThiefAgent::BeginPlay()
{
	Super::BeginPlay(); 
	Goals.Add(NewObject<UEvadeDetectionGoal>(this));
	Goals.Add(NewObject<URunAwayGoal>(this));

	AvailableAction.Add(NewObject<UStealAction>(this));
	AvailableAction.Add(NewObject<UHideAction>(this));
	AvailableAction.Add(NewObject<URunAwayAction>(this));
	WorldState = UWorldState::GetInstance();
	Beliefs = NewObject<UThiefAgentBeliefs>(this);
	Sensor = NewObject<UThiefAgentSesnor>(GetOwner(), UThiefAgentSesnor::StaticClass(), TEXT("Sensor"));
	Sensor->SetOwnerAgent(this);
	Sensor->RegisterComponent();
	if (UThiefAgentSesnor* PatrolSensor = Cast<UThiefAgentSesnor>(Sensor))
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
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &UThiefAgent::OnPerceptionUpdated);
	StimuliSourceComponent = NewObject<UAIPerceptionStimuliSourceComponent>(GetOwner(), UAIPerceptionStimuliSourceComponent::StaticClass(), TEXT("StimuliSourceComponent"));
	StimuliSourceComponent->RegisterComponent();
	StimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
	StimuliSourceComponent->RegisterWithPerceptionSystem();
}
