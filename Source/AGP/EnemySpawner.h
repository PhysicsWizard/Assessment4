// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class APlayerCharacter;

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
	bool IsImmuneToSpecialKills(int SpecialKillsPerformed);

	TArray<FVector> PossibleSpawnLocations;
	void PopulateSpawnLocations();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemy();

};
