// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h" 
#include "EnemyAgentBeliefs.h" 
#include "AGP/Characters/HealthComponent.h"
#include "GOAP base/Agent.h"
#include "EnemyAgent.generated.h"


struct FEnemyStats;
class AEnemyCharacter;
/**
 * 
 */
UCLASS()

class AGP_API UEnemyAgent : public UAgent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void PerformAction() override;
	bool PlanActions() override;
	void SetTheOwner(AEnemyCharacter* EnemyCharacter);
	UHealthComponent* GetHealthComponent();
	virtual UEnemyAgentBeliefs* GetBeliefs() const;
	virtual UEnemyAgentBeliefs* GetBeliefs() override;
	AEnemyCharacter* GetEnemyCharacterComponent();
	FEnemyStats* GetEnemyStats();
	
protected:
	UEnemyAgent();
	virtual void BeginPlay() override;
	void ManageHealthBeliefs();
	void ManageSensedCharacters();
	
private:

	UPROPERTY()
	AEnemyCharacter* EnemyCharacterComponent;
	UPROPERTY()
	UHealthComponent* HealthComponent;
};
