// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h" 
#include "EnemyAgentBeliefs.h"
#include "GOAP base/Agent.h"
#include "EnemyAgent.generated.h"

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
	
protected:
	UEnemyAgent();
	virtual void BeginPlay() override;
	virtual UEnemyAgentBeliefs* GetBeliefs() const;
	virtual UEnemyAgentBeliefs* GetBeliefs() override;
private:
	void SetUpPerception();
};
