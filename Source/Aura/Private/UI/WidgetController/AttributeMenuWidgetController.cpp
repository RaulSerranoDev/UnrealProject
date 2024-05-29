// Copyright RaulSerranoDev


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/GameAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/GamePlayerState.h"
#include "AbilitySystem/GameAbilitySystemComponent.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	check(AttributeInfo);

	for (auto& Pair : GetAS()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value);
	}

	AttributePointsChangedDelegate.Broadcast(GetPS()->GetAttributePoints());
	SpellPointsChangedDelegate.Broadcast(GetPS()->GetSpellPoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	check(AttributeInfo);

	for (auto& Pair : GetAS()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data) {
				BroadcastAttributeInfo(Pair.Key, Pair.Value);
			}
		);
	}

	GetPS()->OnAttributePointsChangedDelegate.AddLambda(
		[this](const int32& NewAttributePoints)
		{
			AttributePointsChangedDelegate.Broadcast(NewAttributePoints);
		}
	);
	GetPS()->OnSpellPointsChangedDelegate.AddLambda(
		[this](const int32& NewSpellPoints)
		{
			SpellPointsChangedDelegate.Broadcast(NewSpellPoints);
		}
	);
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetASC()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag, const FGameplayAttribute& Attribute) const
{
	FGameAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
