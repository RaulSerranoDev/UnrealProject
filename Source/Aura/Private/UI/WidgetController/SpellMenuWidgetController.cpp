// Copyright RaulSerranoDev


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/GamePlayerState.h"
#include "GameGameplayTags.h"

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

	GetASC()->AbilityEquipped.AddUObject(this, &ThisClass::OnAbilityEquipped);

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
	return GetASC()->GetDescriptionsByAbilityTag(SelectedAbility, AbilityInfo, OutDescription, OutNextLevelDescription);
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

	const FGameplayTag SelectedStatus = GetASC()->GetStatusFromAbilityTag(SelectedAbility);
	if (SelectedStatus.MatchesTagExact(TAG_Abilities_Status_Equipped))
	{
		SelectedSlot = GetASC()->GetInputTagFromAbilityTag(SelectedAbility);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityTypeTag)
{
	if (!bWaitingForEquipSelection) return;
	// Check selected ability against the slot's ability type. 
	// (don't equip an offensive spell in a passive slot and vice versa)
	const FGameplayTag SelectedAbilityTypeTag = AbilityInfo->FindAbilityInfoForTag(SelectedAbility).AbilityTypeTag;
	if (!SelectedAbilityTypeTag.MatchesTagExact(AbilityTypeTag)) return;

	GetASC()->ServerEquipAbility(SelectedAbility, SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	bWaitingForEquipSelection = false;

	FGameAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = TAG_Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = TAG_Abilities_None;
	// Broadcast empty info if PreviousSlot is a valid Slot. Only if equipping an already-equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FGameAbilityInfo SlotInfo = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	SlotInfo.StatusTag = Status;
	SlotInfo.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(SlotInfo);

	StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityTypeTag);
}
