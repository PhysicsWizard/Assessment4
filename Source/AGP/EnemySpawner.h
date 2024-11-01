// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class APlayerCharacter;
class ABaseCharacter;

UCLASS()
class AGP_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	UPROPERTY(EditAnywhere)
	float SpawnThreatLevel;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float SpawnTimer = 5.0f;

	APlayerCharacter* PlayerCharacter;

	FActorSpawnParameters SpawnParameters;

	int32 GetEnemySpawnAmount(int ThreatLevel);

	FLinearColor GenerateColour(float AggressionInput, float NoiseSensitivityInput);

	float GenerateAggression(float EnemiesKilledInput);
	float GenerateNoiseSensitivity(float DetectionInput);
	float GetInstaKillChance(int SpecialKillsPerformed);

	TArray<FVector> PossibleSpawnLocations;
	void PopulateSpawnLocations();


	UPROPERTY()
	int32 PlayerKillsInLastMinute = 0;

	UPROPERTY()
	int32 PlayerSpecialKillsInLastMinute = 0;

	UPROPERTY()
	float TimesPlayerDetected = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SpawnEnemy();

	void IncreaseKill(bool bIsSpecialKill);

	void IncreasePlayerDetected();

	void ResetKillsAndDetected();
};
