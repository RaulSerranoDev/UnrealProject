// Copyright RaulSerranoDev


#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"

#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "GameGameplayTags.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UGameAbilitySystemComponent* GameASC = Cast<UGameAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		GameASC->ActivatePassiveEffect.AddUObject(this, &ThisClass::OnPassiveActivate);
		ActivateIfEquipped(GameASC);
	}
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* ASC)
			{
				if (UGameAbilitySystemComponent* GameASC = Cast<UGameAbilitySystemComponent>(ASC))
				{
					GameASC->ActivatePassiveEffect.AddUObject(this, &ThisClass::OnPassiveActivate);
					ActivateIfEquipped(GameASC);
				}
			});
	}
}

void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
	if (AbilityTag.MatchesTagExact(PassiveSpellTag))
	{
		if (bActivate && !IsActive())
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}

void UPassiveNiagaraComponent::ActivateIfEquipped(UGameAbilitySystemComponent* AuraASC)
{
	const bool bStartupAbilitiesGiven = AuraASC->bStartupAbilitiesGiven;
	if (bStartupAbilitiesGiven)
	{
		if (AuraASC->GetStatusFromAbilityTag(PassiveSpellTag) == TAG_Abilities_Status_Equipped)
		{
			Activate();
		}
	}
}
