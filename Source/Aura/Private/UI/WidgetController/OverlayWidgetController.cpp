// Copyright RaulSerranoDev


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/GameAttributeSet.h"
#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UGameAttributeSet* GameAttributeSet = CastChecked<UGameAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(GameAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(GameAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(GameAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(GameAttributeSet->GetMaxMana());

	if (UGameAbilitySystemComponent* ASC = Cast<UGameAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (ASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(ASC);
		}
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UGameAttributeSet* GameAttributeSet = CastChecked<UGameAttributeSet>(AttributeSet);

	UAttributeDelegateTypes::BindToAttributeValueChangeDelegate(AbilitySystemComponent, GameAttributeSet->GetHealthAttribute(), &OnHealthChanged);
	UAttributeDelegateTypes::BindToAttributeValueChangeDelegate(AbilitySystemComponent, GameAttributeSet->GetMaxHealthAttribute(), &OnMaxHealthChanged);
	UAttributeDelegateTypes::BindToAttributeValueChangeDelegate(AbilitySystemComponent, GameAttributeSet->GetManaAttribute(), &OnManaChanged);
	UAttributeDelegateTypes::BindToAttributeValueChangeDelegate(AbilitySystemComponent, GameAttributeSet->GetMaxManaAttribute(), &OnMaxManaChanged);

	if (UGameAbilitySystemComponent* ASC = Cast<UGameAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (ASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(ASC);
		}
		else
		{
			ASC->AbilitiesGivenDelegate.AddUObject(this, &ThisClass::OnInitializeStartupAbilities);
		}

		ASC->EffectAssetTagsDelegate.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					// For example, say that Tag = Message.HealthPotion
					// "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRow.Broadcast(*Row);
					}
				}
			}
		);
	}
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UGameAbilitySystemComponent* ASC)
{
	// TODO: Get information about all given abilities, look up their Ability Info, and broadcast it to widgets.
	if (!ASC->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
		{
			// TODO: Need a way to figure out the ability tag for a given ability spec.
			FGameAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(UGameAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = UGameAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		});

	ASC->ForEachAbility(BroadcastDelegate);
}
