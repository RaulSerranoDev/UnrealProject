// Copyright RaulSerranoDev


#include "AbilitySystem/Abilities/DamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

FGameplayEffectContextHandle UDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	for (TTuple<FGameplayTag, FDamageRange> Pair : DamageTypes)
	{
		const float ScaledMagnitudeMin = Pair.Value.DamageMin.GetValueAtLevel(GetAbilityLevel());
		const float ScaledMagnitudeMax = Pair.Value.DamageMax.GetValueAtLevel(GetAbilityLevel());
		const float ScaledDamage = FMath::RandRange(ScaledMagnitudeMin, ScaledMagnitudeMax);

		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);
	}

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data, UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));

	return DamageSpecHandle.Data->GetContext();
}

void UDamageGameplayAbility::GetDamageRange(const FGameplayTag& DamageTypeTag, const int32& Level, int32& OutMinDamage, int32& OutMaxDamage)
{
	OutMinDamage = FMath::RoundToInt(DamageTypes[DamageTypeTag].DamageMin.GetValueAtLevel(Level));
	OutMaxDamage = FMath::RoundToInt(DamageTypes[DamageTypeTag].DamageMax.GetValueAtLevel(Level));
}
