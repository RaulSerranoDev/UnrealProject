// Copyright RaulSerranoDev


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/GameAttributeSet.h"
#include "AbilitySystem/GameAbilitySystemComponent.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UGameAttributeSet* GameAttributeSet = CastChecked<UGameAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(GameAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(GameAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(GameAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(GameAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UGameAttributeSet* GameAttributeSet = CastChecked<UGameAttributeSet>(AttributeSet);

	BindToAttributeValueChangeDelegate(GameAttributeSet->GetHealthAttribute(), &OnHealthChanged);
	BindToAttributeValueChangeDelegate(GameAttributeSet->GetMaxHealthAttribute(), &OnMaxHealthChanged);
	BindToAttributeValueChangeDelegate(GameAttributeSet->GetManaAttribute(), &OnManaChanged);
	BindToAttributeValueChangeDelegate(GameAttributeSet->GetMaxManaAttribute(), &OnMaxManaChanged);

	Cast<UGameAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
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

void UOverlayWidgetController::BindToAttributeValueChangeDelegate(const FGameplayAttribute& Attribute, FOnValueChangedSignature* AttributeChangeDelegate)
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		Attribute).AddLambda(
			[AttributeChangeDelegate](const FOnAttributeChangeData& Data)
			{
				AttributeChangeDelegate->Broadcast(Data.NewValue);
			}
	);
}
