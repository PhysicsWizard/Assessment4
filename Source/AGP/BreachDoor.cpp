// Fill out your copyright notice in the Description page of Project Settings.


#include "BreachDoor.h"

// Sets default values for this component's properties
UBreachDoor::UBreachDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UBreachDoor::BreachDoor()
{
	if (AActor* OwningActor = GetOwner())
	{
		OwningActor->Destroy();
	}
}


// Called when the game starts
void UBreachDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBreachDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

