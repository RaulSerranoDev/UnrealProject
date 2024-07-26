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

	GetASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
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

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag AbilityTypeTag = AbilityInfo->FindAbilityInfoForTag(SelectedAbility).AbilityTypeTag;
		StopWaitingForEquipDelegate.Broadcast(AbilityTypeTag);
		bWaitingForEquipSelection = false;
	}

	SelectedAbility = AbilityTag;
}

bool USpellMenuWidgetController::GetSelectedAbilityDescriptions(FString& OutDescription, FString& OutNextLevelDescription)
{
	return GetASC()->GetDescriptionsByAbilityTag(SelectedAbility, OutDescription, OutNextLevelDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (!GetASC()) return;

	GetASC()->ServerSpendSpellPoint(SelectedAbility);
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityTypeTag = AbilityInfo->FindAbilityInfoForTag(SelectedAbility).AbilityTypeTag;

	WaitForEquipDelegate.Broadcast(AbilityTypeTag);
	bWaitingForEquipSelection = true;
}
