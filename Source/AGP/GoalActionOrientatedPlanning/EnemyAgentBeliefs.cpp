// Fill out your copyright notice in the Description page of Project Settings.


#include "AGP/GoalActionOrientatedPlanning/EnemyAgentBeliefs.h"

#include "AGP/Characters/EnemyCharacter.h"
#include "AGP/Characters/HealthComponent.h"

class AEnemyCharacter;

UEnemyAgentBeliefs::UEnemyAgentBeliefs()
{
	BeliefsState.Add("TargetSpotted", false);
	BeliefsState.Add("AttackingTarget", false);
	BeliefsState.Add("InDangerOfDeath", false);
	BeliefsState.Add("IsHealing", false);
	BeliefsState.Add("HasFullHealth", false);
	BeliefsState.Add("WithinRange", false);
	BeliefsState.Add("SafeDistanceToHeal", false);
	BeliefsStateVectors.Add("TargetPosition", FVector::ZeroVector);
	BeliefsStateVectors.Add("LastKnownTargetPosition", FVector::ZeroVector);
}

void UEnemyAgentBeliefs::SetCurrentHealthPercentage(const float percentage)
{
	CurrentHealthPercentage = percentage;
}

float UEnemyAgentBeliefs::GetCurrentHealthPercentage() const
{
	return CurrentHealthPercentage;
}

bool UEnemyAgentBeliefs::bIsClose() const
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOuter()->GetOuter());
	float Distance = FVector::Dist(EnemyCharacter->GetActorLocation(), GetBeliefsStateVectors()["LastKnownTargetPosition"]);
	bool bSafeDistance = Distance <= 300.0f;
	GetBeliefsState()["SafeDistanceToHeal"] = bSafeDistance;
	return bSafeDistance;
}

bool UEnemyAgentBeliefs::bInDangerOfDeath() const
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const float HealthPercentage = EnemyAgent->GetHealthComponent()->GetCurrentHealthPercentage();
	const bool bInDangerOfDeath = HealthPercentage <= 0.31f;
	GetBeliefsState()["InDangerOfDeath"] = bInDangerOfDeath;
	return bInDangerOfDeath;
}

bool UEnemyAgentBeliefs::bHasFullHealth() const
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const float HealthPercentage = EnemyAgent->GetHealthComponent()->GetCurrentHealthPercentage();
	bool bHasFullHealth = HealthPercentage >=1.0f;
	GetBeliefsState()["HasFullHealth"] = bHasFullHealth;
	return bHasFullHealth;
}

bool UEnemyAgentBeliefs::bWithinHealthRangeTolerance(float tolerance) const
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const float HealthPercentage = EnemyAgent->GetHealthComponent()->GetCurrentHealthPercentage();
	return HealthPercentage >= tolerance;
}

void UEnemyAgentBeliefs::UpdateBeliefs()
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOuter()->GetOuter());
	const float HealthPercentage = EnemyAgent->GetHealthComponent()->GetCurrentHealthPercentage();
	bool bHasFullHealth = HealthPercentage >=1.0f;
	GetBeliefsState()["HasFullHealth"] = bHasFullHealth;

	const bool bInDangerOfDeath = HealthPercentage <= 0.31f;
	GetBeliefsState()["InDangerOfDeath"] = bInDangerOfDeath;
	
	float Distance = FVector::Dist(EnemyCharacter->GetActorLocation(), GetBeliefsStateVectors()["LastKnownTargetPosition"]);
	bool bSafeDistance = Distance <= 300.0f;
	GetBeliefsState()["SafeDistanceToHeal"] = bSafeDistance;

}


void UEnemyAgentBeliefs::SetTarget(AActor* target)
{
	Target = target;
}

AActor* UEnemyAgentBeliefs::GetTarget() const
{
	return Target;
}
