// Fill out your copyright notice in the Description page of Project Settings.
#include "AGP/GoalActionOrientatedPlanning/HealAction.h"

#include <string>

#include "EnemyAgent.h"
#include "EnemyAgentBeliefs.h"
#include "AGP/Characters/HealthComponent.h"


UHealAction::UHealAction()
{
	cost = 4.0f;
	Preconditions.Add("TargetSpotted", false);
	Preconditions.Add("InDangerOfDeath", true);
	Preconditions.Add("HasFullHealth", false);
	Preconditions.Add("SafeDistanceToHeal", true);
	Effects.Add("InDangerOfDeath", false);
	Effects.Add("HasFullHealth", true);
}

bool UHealAction::IsActionPossible(const UWorldState& WorldState, const UBeliefs& Beliefs)
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const bool bOutOfHarmToHeal =  EnemyAgent->GetBeliefs()->GetBeliefsState()["SafeDistanceToHeal"] == true;
	const bool bInDangerOfDeath = EnemyAgent->GetBeliefs()->GetBeliefsState()["InDangerOfDeath"] == true;
	const bool bHasFullHealth = EnemyAgent->GetBeliefs()->GetBeliefsState()["HasFullHealth"] == false;
	const bool bTargetSpotted = EnemyAgent->GetBeliefs()->GetBeliefsState()["TargetSpotted"] == false;

	return bOutOfHarmToHeal && bInDangerOfDeath && bHasFullHealth && bTargetSpotted;
	
}

void UHealAction::PerformAction()
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	UHealthComponent* EnemyAgentHealthComponent = EnemyAgent->GetHealthComponent();
	EnemyAgentHealthComponent->ApplyHealing(0.5f);
	UE_LOG(LogTemp, Warning, TEXT("Agent healing itself for: %f"), 0.01f); 
	UE_LOG(LogTemp, Warning, TEXT("Agent's health is : %f"),EnemyAgentHealthComponent->GetCurrentHealth()); 
}

bool UHealAction::IsActionComplete() const
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const bool bNoLongerSafeToHeal = EnemyAgent->GetBeliefs()->GetBeliefsState()["SafeDistanceToHeal"] == false;
	const bool bFullyHealed = EnemyAgent->GetHealthComponent()->GetCurrentHealthPercentage() >= 1.0f;
	return bFullyHealed || bNoLongerSafeToHeal;
}

void UHealAction::ApplyEffects(UWorldState& WorldState)
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	EnemyAgent->GetBeliefs()->GetBeliefsState()["InDangerOfDeath"] = false;
	EnemyAgent->GetBeliefs()->GetBeliefsState()["HasFullHealth"] = true;
	Super::ApplyEffects(WorldState);
}

