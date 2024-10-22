// Fill out your copyright notice in the Description page of Project Settings.
#include "AGP/GoalActionOrientatedPlanning/HealAction.h"

#include <string>

#include "EnemyAgent.h"
#include "EnemyAgentBeliefs.h"
#include "AGP/Characters/HealthComponent.h"


UHealAction::UHealAction()
{
	cost = 2.0f;
}

bool UHealAction::IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)
{
	const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(&Beliefs);
	bool bInDangerOfDeath = EnemyBeliefs->bInDangerOfDeath();
	bool bNotHealing = !EnemyBeliefs->GetBeliefsState()["IsHealing"];
	bool bSafeDistanceToHeal = !EnemyBeliefs->bIsClose();

	return bInDangerOfDeath && bNotHealing && bSafeDistanceToHeal;
}

void UHealAction::PerformAction()
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	UHealthComponent* EnemyAgentHealthComponent = EnemyAgent->GetHealthComponent();
	EnemyAgentHealthComponent->ApplyHealing(0.5f);
	UE_LOG(LogTemp, Log, TEXT("Agent healing itself for: %f"), 0.5f); 
	UE_LOG(LogTemp, Log, TEXT("Agent's health is : %f"),EnemyAgentHealthComponent->GetCurrentHealth()); 
}

bool UHealAction::IsActionComplete() const
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	return EnemyAgent->GetHealthComponent()->GetCurrentHealthPercentage() >= 1.0f;
}

void UHealAction::ApplyEffects(UWorldState& WorldState)
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	EnemyAgent->GetBeliefs()->GetBeliefsState()["IsHealing"] = false;
	EnemyAgent->GetBeliefs()->GetBeliefsState()["InDangerOfDeath"] = false;
	
}

