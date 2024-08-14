// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "EnemySpawnPoint.generated.h"

class AEnemyCharacter;

/**
 *
 */
UCLASS()
class AURA_API AEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	TSubclassOf<AEnemyCharacter> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	int32 EnemyLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

};
