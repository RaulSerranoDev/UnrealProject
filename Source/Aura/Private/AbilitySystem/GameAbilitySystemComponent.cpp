// Copyright RaulSerranoDev


#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "GameGameplayTags.h"
#include "AbilitySystem/Abilities/GameGameplayAbility.h"

void UGameAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UGameAbilitySystemComponent::EffectApplied);
}

void UGameAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UGameGameplayAbility* GameAbility = Cast<UGameGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(GameAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UGameAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;

		AbilitySpecInputPressed(AbilitySpec);
		if (!AbilitySpec.IsActive())
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UGameAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;

		AbilitySpecInputReleased(AbilitySpec);
	}
}

void UGameAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
