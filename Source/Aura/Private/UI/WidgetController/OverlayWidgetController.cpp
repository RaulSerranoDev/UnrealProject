// Copyright RaulSerranoDev


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/GameAttributeSet.h"
#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/GamePlayerState.h"
#include "AbilitySystem/Data/LevelUpInfo.h"

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
	AGamePlayerState* PS = CastChecked<AGamePlayerState>(PlayerState);
	OnXPChanged(PS->GetXP());
	OnPlayerLevelChangedDelegate.Broadcast(PS->GetPlayerLevel());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AGamePlayerState* PS = CastChecked<AGamePlayerState>(PlayerState);
	PS->OnXPChangedDelegate.AddUObject(this, &ThisClass::OnXPChanged);
	PS->OnLevelChangedDelegate.AddLambda(
		[this](const int32& NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}
	);

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

void UOverlayWidgetController::OnXPChanged(const int32& NewXP) const
{
	AGamePlayerState* PS = CastChecked<AGamePlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = PS->LevelUpInfo;
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
