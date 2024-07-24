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

	UFUNCTION(BlueprintNativeEvent)
	FString GetDescription(int32 Level);
	UFUNCTION(BlueprintNativeEvent)
	FString GetNextLevelDescription(int32 Level);

	static FString GetLockedDescription(int32 Level);

protected:

	UFUNCTION(BlueprintPure, meta = (HidePin = "Target"))
	int32 GetManaCost(int32 Level = 1) const;

	UFUNCTION(BlueprintPure, meta = (HidePin = "Target"))
	float GetCD(int32 Level = 1) const;

};
