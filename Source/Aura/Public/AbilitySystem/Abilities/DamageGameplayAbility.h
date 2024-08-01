// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GameGameplayAbility.h"
#include "AbilityTypes.h"
#include "DamageGameplayAbility.generated.h"

USTRUCT(BlueprintType)
struct FMinMaxScalableFloat
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat Min = FScalableFloat(0.9f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat Max = FScalableFloat(1.1f);

	float GetValueInRange(float Level) const
	{
		return FMath::RandRange(Min.GetValueAtLevel(Level), Max.GetValueAtLevel(Level));
	}
};

USTRUCT(BlueprintType)
struct FDamageEffect
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMinMaxScalableFloat Damage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMinMaxScalableFloat DebuffChance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMinMaxScalableFloat DebuffDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMinMaxScalableFloat DebuffFrequency;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMinMaxScalableFloat DebuffDuration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DeathImpulseMagnitude = 60.f;
};

/**
 *
 */
UCLASS()
class AURA_API UDamageGameplayAbility : public UGameGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FGameplayEffectContextHandle CauseDamage(AActor* TargetActor);

	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

protected:
	UFUNCTION(BlueprintPure, meta = (HidePin = "Target"))
	void GetDamageRangeAtLevel(FGameplayTag DamageTypeTag, const int32& Level, int32& OutMinDamage, int32& OutMaxDamage);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (Categories = "Damage"))
	TMap<FGameplayTag, FDamageEffect> DamageTypes;

};
