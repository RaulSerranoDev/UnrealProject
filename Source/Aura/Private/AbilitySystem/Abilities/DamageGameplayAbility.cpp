// Copyright RaulSerranoDev


#include "AbilitySystem/Abilities/DamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

FGameplayEffectContextHandle UDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	for (TTuple<FGameplayTag, FDamageEffect> Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.Damage.GetValueInRange(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);
	}

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data, UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));

	return DamageSpecHandle.Data->GetContext();
}

FDamageEffectParams UDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageEffectClass = DamageEffectClass;
	Params.SourceASC = GetAbilitySystemComponentFromActorInfo();
	Params.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.AbilityLevel = GetAbilityLevel();
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackChance = KnockbackChance;
	Params.KnockbackForceMagnitude = KnockbackForceMagnitude;

	checkf(DamageTypes.Num() <= 1, TEXT("GameplayAbility [%s] does not support yet more than 1 Damage Type"), *GetNameSafe(this));

	for (TTuple<FGameplayTag, FDamageEffect> Pair : DamageTypes)
	{
		FDamageEffectType DamageEffectType;

		DamageEffectType.Damage = Pair.Value.Damage.GetValueInRange(Params.AbilityLevel);
		DamageEffectType.DebuffChance = Pair.Value.DebuffChance.GetValueInRange(Params.AbilityLevel);
		DamageEffectType.DebuffDamage = Pair.Value.DebuffDamage.GetValueInRange(Params.AbilityLevel);
		DamageEffectType.DebuffDuration = Pair.Value.DebuffDuration.GetValueInRange(Params.AbilityLevel);
		DamageEffectType.DebuffFrequency = Pair.Value.DebuffFrequency.GetValueInRange(Params.AbilityLevel);
		Params.DamageTypes.Add(Pair.Key, DamageEffectType);
	}

	if (IsValid(TargetActor))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		Rotation.Pitch = 45.f;
		const FVector ToTarget = Rotation.Vector();
		Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		if (Params.KnockbackChance > FMath::RandRange(1, 100))
		{
			Params.KnockbackForce = ToTarget * KnockbackForceMagnitude;
		}
	}

	return Params;
}

float UDamageGameplayAbility::GetDamage(FGameplayTag DamageTypeTag) const
{
	checkf(DamageTypes.Contains(DamageTypeTag), TEXT("GameplayAbility [%s] does not contain DamageType [%s]"), *GetNameSafe(this), *DamageTypeTag.ToString());
	return DamageTypes[DamageTypeTag].Damage.GetValueInRange(GetAbilityLevel());
}

void UDamageGameplayAbility::GetDamageRangeAtLevel(FGameplayTag DamageTypeTag, const int32& Level, int32& OutMinDamage, int32& OutMaxDamage)
{
	checkf(DamageTypes.Contains(DamageTypeTag), TEXT("GameplayAbility [%s] does not contain DamageType [%s]"), *GetNameSafe(this), *DamageTypeTag.ToString());
	OutMinDamage = FMath::RoundToInt(DamageTypes[DamageTypeTag].Damage.Min.GetValueAtLevel(Level));
	OutMaxDamage = FMath::RoundToInt(DamageTypes[DamageTypeTag].Damage.Max.GetValueAtLevel(Level));
}
