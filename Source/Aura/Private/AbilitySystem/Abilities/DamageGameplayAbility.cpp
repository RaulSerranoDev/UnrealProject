// Copyright RaulSerranoDev


#include "AbilitySystem/Abilities/DamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

FGameplayEffectContextHandle UDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	for (TTuple<FGameplayTag, FDamageEffect> Pair : DamageTypes)
	{
		const float ScaledMagnitudeMin = Pair.Value.Damage.Min.GetValueAtLevel(GetAbilityLevel());
		const float ScaledMagnitudeMax = Pair.Value.Damage.Max.GetValueAtLevel(GetAbilityLevel());
		const float ScaledDamage = FMath::RandRange(ScaledMagnitudeMin, ScaledMagnitudeMax);

		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);
	}

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data, UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));

	return DamageSpecHandle.Data->GetContext();
}

void UDamageGameplayAbility::GetDamageRangeAtLevel(FGameplayTag DamageTypeTag, const int32& Level, int32& OutMinDamage, int32& OutMaxDamage)
{
	checkf(DamageTypes.Contains(DamageTypeTag), TEXT("GameplayAbility [%s] does not contain DamageType [%s]"), *GetNameSafe(this), *DamageTypeTag.ToString());
	OutMinDamage = FMath::RoundToInt(DamageTypes[DamageTypeTag].Damage.Min.GetValueAtLevel(Level));
	OutMaxDamage = FMath::RoundToInt(DamageTypes[DamageTypeTag].Damage.Max.GetValueAtLevel(Level));
}
