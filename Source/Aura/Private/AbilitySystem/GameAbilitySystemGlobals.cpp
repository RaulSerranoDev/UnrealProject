// Copyright RaulSerranoDev


#include "AbilitySystem/GameAbilitySystemGlobals.h"
#include "AbilityTypes.h"

FGameplayEffectContext* UGameAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FGameGameplayEffectContext();
}
