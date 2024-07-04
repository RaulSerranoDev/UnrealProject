// Copyright RaulSerranoDev


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/GamePlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	BroadcastAbilityInfo();

	SpellPointsChangedDelegate.Broadcast(GetPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	GetASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
		{
			if (AbilityInfo)
			{
				FGameAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});

	GetPS()->OnSpellPointsChangedDelegate.AddLambda(
		[this](const int32& NewSpellPoints)
		{
			SpellPointsChangedDelegate.Broadcast(NewSpellPoints);
		}
	);
}
