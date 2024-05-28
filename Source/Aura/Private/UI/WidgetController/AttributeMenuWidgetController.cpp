// Copyright RaulSerranoDev


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/GameAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/GamePlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);

	UGameAttributeSet* AS = CastChecked<UGameAttributeSet>(AttributeSet);
	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value);
	}

	AGamePlayerState* PS = CastChecked<AGamePlayerState>(PlayerState);

	AttributePointsChangedDelegate.Broadcast(PS->GetAttributePoints());
	SpellPointsChangedDelegate.Broadcast(PS->GetSpellPoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo);

	UGameAttributeSet* AS = CastChecked<UGameAttributeSet>(AttributeSet);
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data) {
				BroadcastAttributeInfo(Pair.Key, Pair.Value);
			}
		);
	}

	AGamePlayerState* PS = CastChecked<AGamePlayerState>(PlayerState);
	PS->OnAttributePointsChangedDelegate.AddLambda(
		[this](const int32& NewAttributePoints)
		{
			AttributePointsChangedDelegate.Broadcast(NewAttributePoints);
		}
	);
	PS->OnSpellPointsChangedDelegate.AddLambda(
		[this](const int32& NewSpellPoints)
		{
			SpellPointsChangedDelegate.Broadcast(NewSpellPoints);
		}
	);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag, const FGameplayAttribute& Attribute) const
{
	FGameAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
