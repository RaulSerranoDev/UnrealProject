// Copyright RaulSerranoDev


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"
#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* ASC, const FGameplayTag& CooldownTag)
{
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = ASC;
	WaitCooldownChange->CooldownTag = CooldownTag;
	if (!IsValid(ASC) || !CooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	// To know when a cooldown effect has been applied
	ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(
		WaitCooldownChange,
		&ThisClass::OnActiveEffectAdded
	);

	// To know when a cooldown has ended (Cooldown Tag has been removed)
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
		WaitCooldownChange,
		&ThisClass::CooldownTagChanged
	);

	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (!IsValid(ASC)) return;
	ASC->RegisterGameplayTagEvent(CooldownTag).RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	if (!SpecApplied.CapturedRelevantAttributes.HasNonSnapshottedAttributes()) return;

	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);
	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if (!TimesRemaining.IsEmpty())
		{
			float TimeRemaining = FMath::Max(TimesRemaining);
			CooldownStart.Broadcast(TimeRemaining);
		}
	}
}

void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}
