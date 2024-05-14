// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GameGameplayAbility.h"
#include "SummonAbility.generated.h"

/**
 *
 */
UCLASS()
class AURA_API USummonAbility : public UGameGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations(bool bShowDebug = false);

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	int32 NumMinions = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MinSpawnDistance = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MaxSpawnDistance = 250.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnSpreadAngle = 90.f;
};
