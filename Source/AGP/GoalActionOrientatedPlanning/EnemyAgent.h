// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h" 
#include "EnemyAgentBeliefs.h" 
#include "AGP/Characters/HealthComponent.h"
#include "GOAP base/Agent.h"
#include "EnemyAgent.generated.h"

class AEnemyCharacter;
/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AGP_API UEnemyAgent : public UAgent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void PerformAction() override;
	bool PlanActions() override;
	void SetTheOwener(AEnemyCharacter* EnemyCharacter);
	UHealthComponent* GetHealthComponent();
	virtual UEnemyAgentBeliefs* GetBeliefs() const;
	virtual UEnemyAgentBeliefs* GetBeliefs() override;
	AEnemyCharacter* GetEnemyCharacterComponent();
	
protected:
	UEnemyAgent();
	virtual void BeginPlay() override;
	void ManageHealthBeliefs();
	void ManageSensedCharacters();
	
private:

	UPROPERTY(EditAnywhere)
	AEnemyCharacter* EnemyCharacterComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;
};
