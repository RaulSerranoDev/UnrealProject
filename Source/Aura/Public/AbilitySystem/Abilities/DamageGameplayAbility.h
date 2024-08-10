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

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

	UFUNCTION(BlueprintPure, meta = (HidePin = "Target"))
	float GetDamage(FGameplayTag DamageTypeTag) const;

protected:
	UFUNCTION(BlueprintPure, meta = (HidePin = "Target"))
	void GetDamageRangeAtLevel(FGameplayTag DamageTypeTag, const int32& Level, int32& OutMinDamage, int32& OutMaxDamage);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (Categories = "Damage"))
	TMap<FGameplayTag, FDamageEffect> DamageTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float DeathImpulseMagnitude = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (ClampMin = 0, ClampMax = 100))
	float KnockbackChance = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float KnockbackForceMagnitude = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FVector RadialDamageOrigin = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (ClampMin = 0, ClampMax = 100))
	float RadialDamageMinPercentage = 0.f;

};
