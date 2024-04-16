// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameGameplayAbility.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UGameGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;
};
