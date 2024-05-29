// Copyright RaulSerranoDev


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/GameAttributeSet.h"
#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "Player/GamePlayerState.h"
#include "AbilitySystem/Data/LevelUpInfo.h"

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
	OnPlayerLevelChangedDelegate.Broadcast(GetPS()->GetPlayerLevel());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetPS()->OnXPChangedDelegate.AddUObject(this, &ThisClass::OnXPChanged);
	GetPS()->OnLevelChangedDelegate.AddLambda(
		[this](const int32& NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}
	);

	UAttributeDelegateTypes::BindToAttributeValueChangeDelegate(AbilitySystemComponent, GetAS()->GetHealthAttribute(), &OnHealthChanged);
	UAttributeDelegateTypes::BindToAttributeValueChangeDelegate(AbilitySystemComponent, GetAS()->GetMaxHealthAttribute(), &OnMaxHealthChanged);
	UAttributeDelegateTypes::BindToAttributeValueChangeDelegate(AbilitySystemComponent, GetAS()->GetManaAttribute(), &OnManaChanged);
	UAttributeDelegateTypes::BindToAttributeValueChangeDelegate(AbilitySystemComponent, GetAS()->GetMaxManaAttribute(), &OnMaxManaChanged);

	if (GetASC())
	{
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
