// Copyright RaulSerranoDev


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/GameAttributeSet.h"
#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "Player/GamePlayerState.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameGameplayTags.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAS()->GetMaxMana());

	if (GetASC())
	{
		if (GetASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
	}

	OnXPChanged(GetPS()->GetXP());
	OnPlayerLevelChangedDelegate.Broadcast(GetPS()->GetPlayerLevel(), false);
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetPS()->OnXPChangedDelegate.AddUObject(this, &ThisClass::OnXPChanged);
	GetPS()->OnLevelChangedDelegate.AddLambda(
		[this](const int32& NewLevel, bool bLevelUp)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel, bLevelUp);
		}
	);

	UAttributeDelegateTypes::BindToAttributeValueChangeDelegate(AbilitySystemComponent, GetAS()->GetHealthAttribute(), &OnHealthChanged);
	UAttributeDelegateTypes::BindToAttributeValueChangeDelegate(AbilitySystemComponent, GetAS()->GetMaxHealthAttribute(), &OnMaxHealthChanged);
	UAttributeDelegateTypes::BindToAttributeValueChangeDelegate(AbilitySystemComponent, GetAS()->GetManaAttribute(), &OnManaChanged);
	UAttributeDelegateTypes::BindToAttributeValueChangeDelegate(AbilitySystemComponent, GetAS()->GetMaxManaAttribute(), &OnMaxManaChanged);

	if (GetASC())
	{
		GetASC()->AbilityEquipped.AddUObject(this, &ThisClass::OnAbilityEquipped);

		if (GetASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetASC()->AbilitiesGivenDelegate.AddUObject(this, &ThisClass::BroadcastAbilityInfo);
		}

		GetASC()->EffectAssetTagsDelegate.AddLambda(
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

void UOverlayWidgetController::OnXPChanged(const int32& NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetPS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo. Please fill out GamePlayerState Blueprint"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInfo.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 XPRequirement = LevelUpInfo->LevelUpInfo[Level].XPRequirement;
		const int32 PreviousXPRequirement = LevelUpInfo->LevelUpInfo[Level - 1].XPRequirement;

		const int32 DeltaLevelRequirement = XPRequirement - PreviousXPRequirement;
		const int32 XPForThisLevel = NewXP - PreviousXPRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
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
}
