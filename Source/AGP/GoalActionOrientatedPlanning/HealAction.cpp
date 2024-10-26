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
	const UEnemyAgentBeliefs* EnemyBeliefs = Cast<UEnemyAgentBeliefs>(&Beliefs);
	const bool bInDangerOfDeath = EnemyBeliefs->bInDangerOfDeath();
	const bool bNotHealing = !EnemyBeliefs->GetBeliefsState()["IsHealing"];
	const bool bSafeDistanceToHeal = !EnemyBeliefs->bIsClose();
	const bool bEnemyNotVisible = EnemyBeliefs->GetTarget() == nullptr;

	return bInDangerOfDeath && bNotHealing && bSafeDistanceToHeal && bEnemyNotVisible;
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
	Super::ApplyEffects(WorldState);
}

