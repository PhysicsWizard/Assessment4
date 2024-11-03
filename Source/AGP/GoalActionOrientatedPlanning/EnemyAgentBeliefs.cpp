// Fill out your copyright notice in the Description page of Project Settings.


#include "AGP/GoalActionOrientatedPlanning/EnemyAgentBeliefs.h"

#include "EnemyAgent.h"
#include "AGP/Characters/EnemyCharacter.h"
#include "AGP/Characters/HealthComponent.h"

class AEnemyCharacter;

UEnemyAgentBeliefs::UEnemyAgentBeliefs()
{
	BeliefsState.Emplace("TargetSpotted", false);
	BeliefsState.Emplace("AttackingTarget", false);
	BeliefsState.Emplace("InDangerOfDeath", false);
	BeliefsState.Emplace("IsHealing", false);
	BeliefsState.Emplace("HasFullHealth", true);
	BeliefsState.Emplace("WithinRange", false);
	BeliefsState.Emplace("SafeDistanceToHeal", false);
	BeliefsStateVectors.Emplace("TargetPosition", FVector::ZeroVector);
	BeliefsStateVectors.Emplace("LastKnownTargetPosition", FVector::ZeroVector);
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
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOuter()->GetOuter());
	float Distance = FVector::Dist(EnemyCharacter->GetActorLocation(), EnemyAgent->GetBeliefs()->GetBeliefsStateVectors()["LastKnownTargetPosition"]);
	bool bSafeDistance = Distance <= EnemyCharacter->GetNoiseSenitivity();
	EnemyAgent->GetBeliefs()->GetBeliefsState()["SafeDistanceToHeal"] = bSafeDistance;
	return bSafeDistance;
}

bool UEnemyAgentBeliefs::bInDangerOfDeath() const
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	const float HealthPercentage = EnemyAgent->GetHealthComponent()->GetCurrentHealthPercentage();
	const bool bInDangerOfDeath = HealthPercentage <= Cast<AEnemyCharacter>(EnemyAgent->GetOuter())->GetAggressionClamped();
	UE_LOG(LogTemp, Log, TEXT("Enemy Aggression Clamped: %f"),Cast<AEnemyCharacter>(GetOuter()->GetOuter())->GetAggressionClamped() );
	EnemyAgent->GetBeliefs()->GetBeliefsState()["InDangerOfDeath"] = bInDangerOfDeath;
	return bInDangerOfDeath;
}

bool UEnemyAgentBeliefs::bHasFullHealth() const
{
	UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
	float HealthPercentage = EnemyAgent->GetHealthComponent()->GetCurrentHealthPercentage();
	bool bHasFullHealth = HealthPercentage >=1.0f;
	EnemyAgent->GetBeliefs()->GetBeliefsState()["HasFullHealth"] = bHasFullHealth;
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

	const bool bInDangerOfDeath = HealthPercentage <= 0.61f;
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
