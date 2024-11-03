// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.generated.h"

// Forward declarations to avoid needing to #include files in the header of this class.
// When these classes are used in the .cpp file, they are #included there.
class UPawnSensingComponent;
class APlayerCharacter;
class UPathfindingSubsystem;
class UEnemyAgent;

USTRUCT(BlueprintType)
struct FEnemyStats
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	float Aggression;

	UPROPERTY(EditAnywhere)
	float SizeFactor;

	UPROPERTY(EditAnywhere)
	float NoiseSensitivity;

	UPROPERTY(EditAnywhere)
	float InstaKillChance;

	FEnemyStats()
		: Aggression(10.0f), SizeFactor(1.0f), NoiseSensitivity(1.0f), InstaKillChance(0.05f){}
};
 
UCLASS()
class AGP_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UPROPERTY(Replicated, VisibleAnywhere)
	FLinearColor EnemyColourProperty;

	UPROPERTY(Replicated, VisibleAnywhere)
	float EnemyGlowFactor;

	UPROPERTY(Replicated, VisibleAnywhere)
	float EnemyScaleFactor;

	UFUNCTION(NetMulticast,Reliable)
	void Multicast_SetColourAndGlow(FLinearColor EnemyColour, float EnemyGlow);

	UFUNCTION(NetMulticast,Reliable)
	void Multicast_SetMeshSize(float ScaleFactor);


	UFUNCTION(NetMulticast, Reliable)
	void SetStats(FEnemyStats StatsToSet);

	UFUNCTION()
	float GetAggressionClamped();

	UFUNCTION()
	float GetNoiseSenitivity();
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	UEnemyAgent* EnemyAgentComponent;
	UPROPERTY()
	AActor* ItSelf;
	
	/**
	 * Will move the character along the CurrentPath or do nothing to the character if the path is empty.
	 */
	void MoveAlongPath();



	/**
	 * A function bound to the UPawnSensingComponent's OnSeePawn event. This will set the SensedCharacter variable
	 * if the pawn that was sensed was of type APlayerCharacter.
	 * @param SensedActor The pawn that was sensed by the UPawnSensingComponent.
	 */
	UFUNCTION()
	void OnSensedPawn(APawn* SensedActor);
	/**
	 * Will update the SensedCharacter variable based on whether the UPawnSensingComponent has a line of sight to the
	 * Player Character or not. This may cause the SensedCharacter variable to become a nullptr so be careful when using
	 * the SensedCharacter variable.
	 */
	void UpdateSight();

	//UPawnSensor not working for some reason
	void PerformRaycastDetection();

	/**
	 * A pointer to the Pathfinding Subsystem.
	 */
	UPROPERTY()
	UPathfindingSubsystem* PathfindingSubsystem;

	/**
	 * A pointer to the PawnSensingComponent attached to this enemy character.
	 */
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensingComponent;

	/**
	 * A pointer to a PlayerCharacter that can be seen by this enemy character. If this is nullptr then the enemy cannot
	 * see any PlayerCharacter.
	 */
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Detection")
	APlayerCharacter* SensedCharacter = nullptr;

	/**
	 * An array of vectors representing the current path that the agent is traversing along.
	 */
	UPROPERTY(VisibleAnywhere)
	TArray<FVector> CurrentPath; 

	/**
	 * Some arbitrary error value for determining how close is close enough before moving onto the next step in the path.
	 */
	UPROPERTY(EditAnywhere)
	float PathfindingError = 150.0f; // 150 cm from target by default.

	/**
	 * Enemy Stats stored as struct.
	 */
	UPROPERTY(Replicated, EditAnywhere)
	FEnemyStats Stats;
	
	UFUNCTION()
	void OnSeePawnDetected(APawn* SeenPawn);

	
public:
	// reuse these functions to be called in seperate action classes
	void TickEvade(); 
	void TickPatrol();
	void TickEngage();
	void TickEngageStationary();
	void TickAdanceToTarget();
	void TickGoToLocation( FVector& location);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	APlayerCharacter* GetSensedCharacter();
	FEnemyStats GetStats() const;
	FEnemyStats* GetStats();

private:
	
	/**
	 * NOT USED ANYMORE - Was used for TickEvade and TickEngage before we setup the UPawnSensingComponent.
	 * @return A pointer to one APlayerCharacter actor in the world.
	 */
	APlayerCharacter* FindPlayer() const;
	float TimeSinceLastShot = 0.0f;

};
