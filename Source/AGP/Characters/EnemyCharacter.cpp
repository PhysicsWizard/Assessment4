// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "EngineUtils.h"
#include "HealthComponent.h"
#include "PlayerCharacter.h"
#include "AGP/EnemySpawner.h"
#include "AGP/GoalActionOrientatedPlanning/EnemyAgent.h"
#include "AGP/Pathfinding/PathfindingSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("Pawn Sensing Component");
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// DO NOTHING IF NOT ON THE SERVER
	if (GetLocalRole() != ROLE_Authority) return;
	
	PathfindingSubsystem = GetWorld()->GetSubsystem<UPathfindingSubsystem>();
	if (PathfindingSubsystem)
	{
		CurrentPath = PathfindingSubsystem->GetRandomPath(GetActorLocation());
	} 
	if (PawnSensingComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("There is a pawn sensing component"))
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSeePawnDetected);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No pawn sensing component"))
	}
}

void AEnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate these properties.
	DOREPLIFETIME(AEnemyCharacter, EnemyColourProperty);
	DOREPLIFETIME(AEnemyCharacter, EnemyGlowFactor);
	DOREPLIFETIME(AEnemyCharacter, EnemyScaleFactor);
	DOREPLIFETIME(AEnemyCharacter, Stats);
}


APlayerCharacter* AEnemyCharacter::GetSensedCharacter()
{
	return SensedCharacter;
}

void AEnemyCharacter::MoveAlongPath()
{
	// Execute the path. Should be called each tick.

	// If the path is empty do nothing.
	if (CurrentPath.IsEmpty()) return;
	
	// 1. Move towards the current stage of the path.
	//		a. Calculate the direction from the current position to the target of the current stage of the path.
	FVector MovementDirection = CurrentPath[CurrentPath.Num()-1] - GetActorLocation();
	MovementDirection.Normalize();
	//		b. Apply movement in that direction.
	AddMovementInput(MovementDirection);
	// 2. Check if it is close to the current stage of the path then pop it off.
	if (FVector::Distance(GetActorLocation(), CurrentPath[CurrentPath.Num() - 1]) < PathfindingError)
	{
		CurrentPath.Pop();
	}
}

void AEnemyCharacter::TickPatrol()
{
	if (CurrentPath.IsEmpty())
	{
		CurrentPath = PathfindingSubsystem->GetRandomPath(GetActorLocation());
	}
	MoveAlongPath();
}

void AEnemyCharacter::TickEngage()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	if (!SensedCharacter){
		UE_LOG(LogTemp, Warning, TEXT("No Sensed Character"));
		return;
	}
	
	if (CurrentPath.IsEmpty())
	{
		CurrentPath = PathfindingSubsystem->GetPath(GetActorLocation(), SensedCharacter->GetActorLocation());
	}
	MoveAlongPath();
	if (HasWeapon())
	{
		if (WeaponComponent->IsMagazineEmpty())
		{
			Reload();
		}
		if(TimeSinceLastShot >= 0.1f)
		{
			Fire(SensedCharacter->GetActorLocation());
			TimeSinceLastShot = 0.0f;
		}
	}
	TimeSinceLastShot += DeltaTime;
}

void AEnemyCharacter::TickEngageStationary()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	if (!SensedCharacter) return;
	if (HasWeapon())
	{
		if (WeaponComponent->IsMagazineEmpty())
		{
			Reload();
		}
		if(TimeSinceLastShot >= 0.1f)
		{
			Fire(SensedCharacter->GetActorLocation());
			TimeSinceLastShot = 0.0f;
		}
	}
	TimeSinceLastShot += DeltaTime;
}

void AEnemyCharacter::TickAdanceToTarget()
{
	if (!SensedCharacter) return;
	
	if (CurrentPath.IsEmpty())
	{
		CurrentPath = PathfindingSubsystem->GetPath(GetActorLocation(), SensedCharacter->GetActorLocation());
	}
	MoveAlongPath();
}

void AEnemyCharacter::OnSeePawnDetected(APawn* SeenPawn)
{
	if(SeenPawn)
	{
		SensedCharacter = Cast<APlayerCharacter>(SeenPawn);
		if (SensedCharacter)
		{
			// Successfully cast to APlayerCharacter, proceed with any additional logic
			UE_LOG(LogTemp, Log, TEXT("Player detected: %s"), *SensedCharacter->GetName());
		}
	}
	else
	{
		SensedCharacter = nullptr;
	}
}

void AEnemyCharacter::TickEvade()
{
	// Find the player and return if it can't find it.
	//if (!SensedCharacter) return;

	if (CurrentPath.IsEmpty())
	{
		CurrentPath = PathfindingSubsystem->GetPathAway(GetActorLocation(), SensedCharacter->GetActorLocation());
	}
	MoveAlongPath();
}

void AEnemyCharacter::OnSensedPawn(APawn* SensedActor)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(SensedActor))
	{
		SensedCharacter = Player;
	}
}

void AEnemyCharacter::UpdateSight()
{
	if (!SensedCharacter) return;
	if (PawnSensingComponent)
	{
		if (!PawnSensingComponent->HasLineOfSightTo(SensedCharacter))
		{
			SensedCharacter = nullptr;
			//BTW this resets the sensedcharacter, meaning the enemy cannot detect a player
			// so resets the SensedCharacter back to a nullptr
			if(EnemySpawner)
			{
				EnemySpawner->IncreasePlayerDetected();
			}
		}
	}
}

void AEnemyCharacter::PerformRaycastDetection()
{
    FVector StartLocation = GetActorLocation();
    FVector ForwardVector =GetActorForwardVector();

    float FOVAngle = 120.0f;        // Total field of view in degrees
    int32 NumberOfRays = 120;         // Number of line traces

    // Calculate the angle increment between each ray
    float AngleIncrement = FOVAngle / (NumberOfRays - 1);  
    for (int32 i = 0; i < NumberOfRays; ++i)
    {
	    float DetectRang = 1000.0f;
        float AngleDegrees = -FOVAngle / 2 + i * AngleIncrement;
        FVector Direction = ForwardVector.RotateAngleAxis(AngleDegrees, FVector::UpVector);
        FVector EndLocation = StartLocation + (Direction * DetectRang);
    	UE_LOG(LogTemp, Log, TEXT("Searching..."));
        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(this);

        // Perform the line trace
    	FColor LineColor = FColor::Green;
        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);
        if (bHit && HitResult.GetActor() != nullptr)
        {
        	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(HitResult.GetActor()))
        	{
        		SensedCharacter = PlayerCharacter;
        		LineColor = FColor::Red; 
        		break;  
        	}

        }
    	DrawDebugLine(GetWorld(), StartLocation, EndLocation, LineColor, false, 0.2f, 0, 0.5f);
    } 
}


// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
	// DO NOTHING UNLESS IT IS ON THE SERVER
	if (GetLocalRole() != ROLE_Authority) return;
	UpdateSight();
	//Highly inefficient i know but the sensing component isn't working so i am using this one from my previous assignment
	//PerformRaycastDetection(); // Fallback if sensing component fails

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

APlayerCharacter* AEnemyCharacter::FindPlayer() const
{
	APlayerCharacter* Player = nullptr;
	for (TActorIterator<APlayerCharacter> It(GetWorld()); It; ++It)
	{
		Player = *It;
		break;
	}
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find the Player Character in the world."))
	}
	return Player;
}

void AEnemyCharacter::Multicast_SetColourAndGlow_Implementation(FLinearColor EnemyColour, float EnemyGlow)
{
	USkeletalMeshComponent* EnemyMesh = GetMesh();

	if (EnemyMesh)
	{
		int32 MaterialCount = EnemyMesh->GetNumMaterials();
			
		for (int32 i = 0; i < MaterialCount; i++)
		{
			UMaterialInterface* Material = EnemyMesh->GetMaterial(i);
			UE_LOG(LogTemp, Log, TEXT("Material %d: %s"), i, *Material->GetName());
			UMaterialInstanceDynamic* DynamicMaterial = EnemyMesh->CreateAndSetMaterialInstanceDynamic(i);

			if (i == 0)
			{
				DynamicMaterial->SetVectorParameterValue("BaseColor", EnemyColour);
				DynamicMaterial->SetScalarParameterValue("Glow", EnemyGlow);
			}
			else if (i == 1)
			{
				DynamicMaterial->SetVectorParameterValue("BaseColor", EnemyColour);
			}
		}
		UE_LOG(LogTemp, Log, TEXT("Colour Set"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No Mesh Found"));
	}
}


void AEnemyCharacter::Multicast_SetMeshSize_Implementation(float ScaleFactor)
{
	SetActorScale3D(FVector(ScaleFactor,ScaleFactor,ScaleFactor));
}


void AEnemyCharacter::SetStats_Implementation(FEnemyStats StatsToSet)
{
	Stats = StatsToSet;
}

FEnemyStats* AEnemyCharacter::GetStats()
{
	return &Stats;
}

FEnemyStats AEnemyCharacter::GetStats() const
{
	return Stats;
}

float AEnemyCharacter::GetAggressionClamped()
{
	float ClampedAggression = FMath::Clamp(GetStats()->Aggression, 10.0f, 90.0f);
	return 0.3f - (ClampedAggression - 10.0f / 80.0f) * 0.2f;
}

float AEnemyCharacter::GetNoiseSenitivity()
{
	return Stats.NoiseSensitivity;
}




