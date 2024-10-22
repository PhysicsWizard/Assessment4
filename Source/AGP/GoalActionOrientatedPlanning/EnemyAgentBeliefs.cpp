// Fill out your copyright notice in the Description page of Project Settings.


#include "AGP/GoalActionOrientatedPlanning/EnemyAgentBeliefs.h"

UEnemyAgentBeliefs::UEnemyAgentBeliefs()
{
	BeliefsState.Add("TargetSpotted", false);
	BeliefsState.Add("Attacking", false);
	BeliefsState.Add("GoingToDie", false);

	BeliefsStateVectors.Add("TargetPosition", FVector::ZeroVector);
}

void UEnemyAgentBeliefs::SetCurrentHealthPercentage(const float percentage)
{
	CurrentHealthPercentage = percentage;
}

float UEnemyAgentBeliefs::GetCurrentHealthPercentage() const
{
	return CurrentHealthPercentage;
}

void UEnemyAgentBeliefs::SetTarget(AActor* target)
{
	Target = target;
}

AActor* UEnemyAgentBeliefs::GetTarget() const
{
	return Target;
}
