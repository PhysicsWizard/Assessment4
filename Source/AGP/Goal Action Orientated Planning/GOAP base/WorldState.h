// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WorldState.generated.h"

/**
 * 
 */
UCLASS()
class NAVMESH_TUTORIAL_API UWorldState : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere, Category = "GOAP")
	TMap<FString, bool> States;
	TMap<FString, FVector> VectorStates;

	TMap<FString, bool> GetWorldState() ;
	TMap<FString, FVector> GetWorldVectorState() ;
	TMap<FString, bool> GetWorldState() const ;
	TMap<FString, FVector> GetWorldVectorState() const ;
	
	void UpdateState(const TMap<FString, bool>& Effects);
	void UpdateVectorState(const TMap<FString, FVector>& Effects);
	
	void SetState(FString key, bool value);
	void SetVectorState(FString key, FVector value);

	static UWorldState* GetInstance();

	void PrintWorldStates() const;
	UWorldState* Clone();

protected:
	UWorldState();
	virtual void BeginDestroy() override;
	static UWorldState* Instance;
	
};
