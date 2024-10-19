// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

#include "MultiplayerGameMode.h"
#include "Characters/EnemyCharacter.h"
#include "Characters/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	//Set the spawn parameters to ignore collision; this is so that spawns always happen. Also Get a reference to the player. 
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Using Player kill count, determine how many times to spawn an enemy. Use a timer to determine when spawns will happen. 
	if (SpawnTimer <= 0.0f)
	{
		for (int8 i = 0; i < GetEnemySpawnAmount(PlayerCharacter->GetEnemiesKilledInLastMinute()); i++)
		{
			SpawnEnemy();	
		}
		SpawnTimer += 5.0f;
	}

	SpawnTimer -= DeltaTime;
}

void AEnemySpawner::SpawnEnemy()
{
	//Check if using the correct game instance; this is used to get the enemy class for spawning.
	if (const AMultiplayerGameMode* GameInstance = GetWorld()->GetGameInstance<AMultiplayerGameMode>())
	{
		//Find a spawn location around the player. Then Make sure it is above ground.
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		FVector PointInSphere = UKismetMathLibrary::RandomUnitVector() * FMath::RandRange(40.0f, 1000.0f);
		
		
		FVector SpawnPosition = PointInSphere + PlayerLocation;
		SpawnPosition.Z = 500.0f;

		//Spawn Enemy and generate a stats struct for it using helper methods.
		AEnemyCharacter* Enemy = GetWorld()->SpawnActor<AEnemyCharacter>(GameInstance->GetEnemyClass(), SpawnPosition, FRotator::ZeroRotator, SpawnParameters);
		Enemy->SpawnDefaultController();
		FEnemyStats SpawnedEnemyStats;

		SpawnedEnemyStats.Aggression = GenerateAggression(PlayerCharacter->GetEnemiesKilledInLastMinute());
		SpawnedEnemyStats.SizeFactor = FMath::RandRange(0.5f, 2.5f);
		SpawnedEnemyStats.NoiseSensitivity = GenerateNoiseSensitivity(PlayerCharacter->GetTimesDetected());
		SpawnedEnemyStats.ImmuneToInstaKills = IsImmuneToSpecialKills(PlayerCharacter->GetSpecialKillsPerformedInLastMinute());

		Enemy->SetStats(SpawnedEnemyStats);

		
		//Scale enemy size and set the meshes new colour and glow. 
		float ScaleFactor = SpawnedEnemyStats.SizeFactor;
		USkeletalMeshComponent* MeshComponent = nullptr;
		if (Enemy)
		{
			  MeshComponent = Enemy->GetMesh();
		}

		if (MeshComponent)
		{
			int32 MaterialCount = MeshComponent->GetNumMaterials();
			
			for (int32 i = 0; i < MaterialCount; i++)
			{
				UMaterialInterface* Material = MeshComponent->GetMaterial(i);
				UE_LOG(LogTemp, Log, TEXT("Material %d: %s"), i, *Material->GetName());
				UMaterialInstanceDynamic* DynamicMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(i);

				if (i == 0)
				{
					FLinearColor NewColour = GenerateColour(SpawnedEnemyStats.Aggression, SpawnedEnemyStats.NoiseSensitivity);
					DynamicMaterial->SetVectorParameterValue("BaseColor", NewColour);
					DynamicMaterial->SetScalarParameterValue("Glow", SpawnedEnemyStats.Aggression / 100);
				}
				else if (i == 1)
				{
					FLinearColor NewColour = GenerateColour(SpawnedEnemyStats.Aggression, SpawnedEnemyStats.NoiseSensitivity);
					DynamicMaterial->SetVectorParameterValue("BaseColor", NewColour);
				}
			}

			Enemy->SetActorScale3D(FVector(ScaleFactor,ScaleFactor,ScaleFactor));
		}

		
		UE_LOG(LogTemp, Log, TEXT("Spawned"));
	}
}

//Semi-Random spawn amount determined by how many kills the player has gotten in the last minute.
int32 AEnemySpawner::GetEnemySpawnAmount(int ThreatLevel)
{
	int32 AmountToSpawn = 1;

	if (ThreatLevel <= 0.0f)
	{
		AmountToSpawn = 1;
	}

	else if (0.0f < ThreatLevel && ThreatLevel < 5.0f)
	{
		AmountToSpawn = FMath::RandRange(1, 3);
	}

	else if (5.0f <= ThreatLevel && ThreatLevel < 10.0f)
	{
		AmountToSpawn = FMath::RandRange(3, 6);
	}

	else if (ThreatLevel >= 10.0f)
	{
		AmountToSpawn = 8;
	}
	
	return AmountToSpawn;
}

FLinearColor AEnemySpawner::GenerateColour(float AggressionInput, float NoiseSensitivityInput)
{

	float NormalizedAggression = (AggressionInput - 10) / 90.0; // map 10-100 to 0-1
	float Red = 0.3 + 0.7 * NormalizedAggression;          // increase red
	float Green = 0.7 * (1.0 - NormalizedAggression);      // decrease green
	float Blue = 1.0 * (1.0 - NormalizedAggression);       // decrease blue



	float noiseFactor = 0.05f; // Adjust this factor for more/less variation
	Red += FMath::FRandRange(-noiseFactor, noiseFactor);   // Random variation for red
	Green += FMath::FRandRange(-noiseFactor, noiseFactor); // Random variation for green
	Blue += FMath::FRandRange(-noiseFactor, noiseFactor);  // Random variation for blue
	
	FLinearColor ReturnColour(Red, Green,Blue, 1.0f);
	UE_LOG(LogTemp, Log, TEXT("Colour Seed: %f"), Red);
	return ReturnColour;
}

//Semi-Random Aggression determined by how many kills the player has gotten in the last minute.
float AEnemySpawner::GenerateAggression(float EnemiesKilledInput)
{
	float Aggression = 0.0f;
	if (EnemiesKilledInput <= 0)
	{
		Aggression = 10.0f;
		UE_LOG(LogTemp, Log, TEXT("Enemy Aggression: %F"), Aggression);
		return Aggression;
	}

	if (0 < EnemiesKilledInput && 2 >= EnemiesKilledInput)
	{
		Aggression = FMath::RandRange(11.0f, 30.0f);
		UE_LOG(LogTemp, Log, TEXT("Enemy Aggression: %F"), Aggression);
		return Aggression;
	}

	if (2 < EnemiesKilledInput && 5 >= EnemiesKilledInput)
	{
		Aggression = FMath::RandRange(31.0f, 60.0f);
		UE_LOG(LogTemp, Log, TEXT("Enemy Aggression: %F"), Aggression);
		return Aggression;
	}
	
	if (5 < EnemiesKilledInput && 10 >= EnemiesKilledInput)
	{
		Aggression = FMath::RandRange(61.0f, 85.0f);
		UE_LOG(LogTemp, Log, TEXT("Enemy Aggression: %F"), Aggression);
		return Aggression;
	}

	if (EnemiesKilledInput > 10)
	{
		Aggression =  FMath::RandRange(86.0f, 100.0f);
		UE_LOG(LogTemp, Log, TEXT("Enemy Aggression: %F"), Aggression);
		return Aggression;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get Aggression"));
		return Aggression;
	}
}
//Semi-Random Sensitivity To Noise (functionally not implemented as I am only concerned with the visuals) determined by how many times player has been detected before.
float AEnemySpawner::GenerateNoiseSensitivity(float DetectionInput)
{
	float NoiseSensitivity = 0.0f;
	if (DetectionInput <= 0)
	{
		NoiseSensitivity = 10.0f;
		UE_LOG(LogTemp, Log, TEXT("Enemy NoiseSensitivity: %F"), NoiseSensitivity);
		return NoiseSensitivity;
	}

	if (0 < DetectionInput && 2 >= DetectionInput)
	{
		NoiseSensitivity = FMath::RandRange(11.0f, 30.0f);
		UE_LOG(LogTemp, Log, TEXT("Enemy NoiseSensitivity: %F"), NoiseSensitivity);
		return NoiseSensitivity;
	}

	if (2 < DetectionInput && 5 >= DetectionInput)
	{
		NoiseSensitivity = FMath::RandRange(31.0f, 60.0f);
		UE_LOG(LogTemp, Log, TEXT("Enemy NoiseSensitivity: %F"), NoiseSensitivity);
		return NoiseSensitivity;
	}
	
	if (5 < DetectionInput && 10 >= DetectionInput)
	{
		NoiseSensitivity = FMath::RandRange(61.0f, 85.0f);
		UE_LOG(LogTemp, Log, TEXT("Enemy NoiseSensitivity: %F"), NoiseSensitivity);
		return NoiseSensitivity;
	}

	if (DetectionInput > 10)
	{
		NoiseSensitivity =  FMath::RandRange(86.0f, 100.0f);
		UE_LOG(LogTemp, Log, TEXT("Enemy NoiseSensitivity: %F"), NoiseSensitivity);
		return NoiseSensitivity;
	}
		UE_LOG(LogTemp, Error, TEXT("Failed to get NoiseSensitivity"));
		return NoiseSensitivity;
}

//Semi-Random chance for the enemy to be immune to special kills (functionally not implemented as I am only concerned with visuals) determined by number of special kills player has gotten in last minute.
bool AEnemySpawner::IsImmuneToSpecialKills(int SpecialKillsPerformed)
{
	if (SpecialKillsPerformed <= 0)
	{
		return false;
	}

	if (SpecialKillsPerformed == 1)
	{
		return UKismetMathLibrary::RandomBoolWithWeight(0.05f);
	}

	if (SpecialKillsPerformed == 2)
	{
		return UKismetMathLibrary::RandomBoolWithWeight(0.1f);
	}
	
	if (SpecialKillsPerformed == 3)
	{
		return UKismetMathLibrary::RandomBoolWithWeight(0.25f);
	}

	if (SpecialKillsPerformed >= 4)
	{
		return UKismetMathLibrary::RandomBoolWithWeight(0.3f);
	}

	return false;
}







