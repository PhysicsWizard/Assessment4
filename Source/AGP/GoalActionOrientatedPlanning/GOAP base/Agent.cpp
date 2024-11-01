// Fill out your copyright notice in the Description page of Project Settings.


#include "Agent.h"
#include "Planner.h"     
#include "Action.h"      
#include "AgentSensor.h"
#include "AIController.h"
#include "Goal.h"      
#include "WorldState.h" 
#include "Beliefs.h"    



// Sets default values for this component's properties
UAgent::UAgent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	 
	Planner =  NewObject<UPlanner>();
}


// Called when the game starts
void UAgent::BeginPlay()
{
	Super::BeginPlay();
	WorldState = UWorldState::GetInstance(); 
	// Initialize Planner
	if (!Planner)
	{
		Planner = NewObject<UPlanner>();
	}

	// Safely get AIController
	AIController = Cast<AAIController>(GetOwner()->GetInstigatorController());
}


// Called every frame
void UAgent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UE_LOG(LogTemp, Log, TEXT("Performing..."));
	if(GoalAchieved())
	{
		PlanActions();
	}
	else
	{
		 
		PerformAction();
	}
	// ... 
}

bool UAgent::PlanActions()
{
	CurrentPlan = Planner->FindBestPlan(this, *WorldState, GetBeliefsConst()); 
	return CurrentPlan.Num() > 0;
}

void UAgent::PerformAction()
{
	if (CurrentPlan.Num() == 0)
	{
		//UE_LOG(LogTemp, Log, TEXT("Current Plan empty boss"));
		return;  
	}
	//UE_LOG(LogTemp, Log, TEXT("Current Plan filled up boss"));
	UAction* CurrentAction = CurrentPlan[0];  
	if (CurrentAction->IsActionComplete())
	{
		CurrentAction->ApplyEffects(*WorldState);
		CurrentPlan.RemoveAt(0);  
	}
	else
	{
		CurrentAction->PerformAction();  
	}
}

bool UAgent::GoalAchieved()
{
	return CurrentPlan.Num() == 0;
}

void UAgent::UpdateBelief(const FString& Key, bool Value)
{
	Beliefs->GetBeliefsState().Add(Key, Value);
}

void UAgent::UpdateBelief(const FString& Key, FVector Value)
{
	Beliefs->GetBeliefsStateVectors().Add(Key, Value);
}

void UAgent::DebugDrawSightPerception()
{
	if (PerceptionComponent)
	{
		// Get the sight radius and lose sight radius
		const float SightRadius = SightConfig->SightRadius;
		const float LoseSightRadius = SightConfig->LoseSightRadius;

		// Get the character's location and forward vector
		FVector Location = GetOwner()->GetActorLocation();
		FVector ForwardVector = GetOwner()->GetActorForwardVector();

		// Draw the sight radius
		DrawDebugSphere(GetWorld(), Location, SightRadius, 12, FColor::Green, false, -1, 0, 1);
		DrawDebugSphere(GetWorld(), Location, LoseSightRadius, 12, FColor::Red, false, -1, 0, 1);

		// Draw the peripheral vision cone
		float PeripheralAngle =  SightConfig->PeripheralVisionAngleDegrees;
		DrawDebugCone(GetWorld(), Location, ForwardVector, SightRadius, FMath::DegreesToRadians(PeripheralAngle), 0.0f, 12, FColor::Blue, false, -1, 0, 1);
	}
}


UGoal* UAgent::GetCurrentGoal()
{
	return CurrentGoal;
}

TArray<UAction*> UAgent::GetAvailableAction()
{
	return AvailableAction;
}

TArray<UAction*> UAgent::GetCurrentPlan()
{
	return CurrentPlan;
}

TArray<UGoal*> UAgent::GetGoals()
{
	return Goals;
}

UWorldState* UAgent::GetWorldState()
{
	return WorldState;
}

UBeliefs& UAgent::GetBeliefsConst() const
{
	return *Beliefs;
}


UBeliefs* UAgent::GetBeliefs()
{
	return Beliefs;
}

AAIController* UAgent::GetAaiController()
{
	return AIController;
}

float UAgent::GetNormalSpeed()
{
	return NormalSpeed;
}

float UAgent::GetRunSpeed()
{
	return RunSpeed;
}

void UAgent::PerformDetection()
{
	
}

