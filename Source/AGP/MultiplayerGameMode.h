// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/EnemyCharacter.h"
#include "GameFramework/GameMode.h"
#include "MultiplayerGameMode.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class AGP_API AMultiplayerGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	void RespawnPlayer(AController* Controller);

	UClass* GetEnemyClass() const;

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlayerCharacter> PlayerCharacterClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Enemy Classes")
    TSubclassOf<AEnemyCharacter> EnemyClass;

	
};
