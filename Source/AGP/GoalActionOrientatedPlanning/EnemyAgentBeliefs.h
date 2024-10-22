// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAP base/Beliefs.h"
#include "EnemyAgentBeliefs.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API UEnemyAgentBeliefs : public UBeliefs
{
	GENERATED_BODY()

protected:
	UEnemyAgentBeliefs();

	UPROPERTY()
	float CurrentHealthPercentage;
	UPROPERTY()
	AActor* Target;
	

public:
	void SetCurrentHealthPercentage(const float percentage);
	float GetCurrentHealthPercentage() const;

	void SetTarget(AActor* target);
	AActor* GetTarget() const;
};