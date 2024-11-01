#include "AGP/GoalActionOrientatedPlanning/EnemyAgentBeliefs.h"

#include "AGP/Characters/EnemyCharacter.h"
#include "AGP/Characters/HealthComponent.h"
#include "Engine/Engine.h" // For logging

class AEnemyCharacter;

UEnemyAgentBeliefs::UEnemyAgentBeliefs()
{
    // Initialize BeliefsState
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
    UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
    if (!EnemyAgent)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyAgent is null in bIsClose()"));
        return false;
    }

    const AEnemyCharacter* EnemyCharacter = EnemyAgent->GetEnemyCharacterComponent();
    if (!EnemyCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyCharacter is null in bIsClose()"));
        return false;
    }

    const FVector TargetPosition = GetBeliefsStateVectors().FindRef("LastKnownTargetPosition");
    const float Distance = FVector::Dist(EnemyCharacter->GetActorLocation(), TargetPosition);
    const bool bSafeDistance = Distance <= 300.0f;
    GetBeliefsState()["SafeDistanceToHeal"] = bSafeDistance;

    return bSafeDistance;
}

bool UEnemyAgentBeliefs::bInDangerOfDeath() const
{
    UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
    if (!EnemyAgent)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyAgent is null in bInDangerOfDeath()"));
        return false;
    }

    UHealthComponent* HealthComponent = EnemyAgent->GetHealthComponent();
    if (!HealthComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("HealthComponent is null in bInDangerOfDeath()"));
        return false;
    }

    const float HealthPercentage = HealthComponent->GetCurrentHealthPercentage();
    const bool bInDangerOfDeath = HealthPercentage <= 0.31f;
    GetBeliefsState()["InDangerOfDeath"] = bInDangerOfDeath;

    return bInDangerOfDeath;
}

bool UEnemyAgentBeliefs::bHasFullHealth() const
{
    UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
    if (!EnemyAgent)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyAgent is null in bHasFullHealth()"));
        return false;
    }

    UHealthComponent* HealthComponent = EnemyAgent->GetHealthComponent();
    if (!HealthComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("HealthComponent is null in bHasFullHealth()"));
        return false;
    }

    const float HealthPercentage = HealthComponent->GetCurrentHealthPercentage();
    const bool bHasFullHealth = HealthPercentage >= 1.0f;
    GetBeliefsState()["HasFullHealth"] = bHasFullHealth;

    return bHasFullHealth;
}

bool UEnemyAgentBeliefs::bWithinHealthRangeTolerance(float tolerance) const
{
    UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
    if (!EnemyAgent)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyAgent is null in bWithinHealthRangeTolerance()"));
        return false;
    }

    UHealthComponent* HealthComponent = EnemyAgent->GetHealthComponent();
    if (!HealthComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("HealthComponent is null in bWithinHealthRangeTolerance()"));
        return false;
    }

    const float HealthPercentage = HealthComponent->GetCurrentHealthPercentage();
    return HealthPercentage >= tolerance;
}

void UEnemyAgentBeliefs::UpdateBeliefs()
{
    UEnemyAgent* EnemyAgent = Cast<UEnemyAgent>(GetOuter());
    if (!EnemyAgent)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyAgent is null in UpdateBeliefs()"));
        return;
    }

    AEnemyCharacter* EnemyCharacter = EnemyAgent->GetEnemyCharacterComponent();
    if (!EnemyCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyCharacter is null in UpdateBeliefs()"));
        return;
    }

    UHealthComponent* HealthComponent = EnemyAgent->GetHealthComponent();
    if (!HealthComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("HealthComponent is null in UpdateBeliefs()"));
        return;
    }

    const float HealthPercentage = HealthComponent->GetCurrentHealthPercentage();

    const bool bHasFullHealth = HealthPercentage >= 1.0f;
    GetBeliefsState()["HasFullHealth"] = bHasFullHealth;

    const bool bInDangerOfDeath = HealthPercentage <= 0.31f;
    GetBeliefsState()["InDangerOfDeath"] = bInDangerOfDeath;

    const FVector TargetPosition = GetBeliefsStateVectors().FindRef("LastKnownTargetPosition");
    const float Distance = FVector::Dist(EnemyCharacter->GetActorLocation(), TargetPosition);
    const bool bSafeDistance = Distance <= 300.0f;
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
