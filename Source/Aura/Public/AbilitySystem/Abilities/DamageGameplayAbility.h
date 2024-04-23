// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GameGameplayAbility.h"
#include "DamageGameplayAbility.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UDamageGameplayAbility : public UGameGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat Damage;

};