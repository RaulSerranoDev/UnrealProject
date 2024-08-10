// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DamageGameplayAbility.h"
#include "FireBlast.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UFireBlast : public UDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure, meta = (HidePin = "Target"))
	int32 GetNumFireBalls(int32 Level = 1) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	int32 NumFireBalls = 12;
};
