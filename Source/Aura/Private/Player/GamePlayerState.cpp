// Copyright RaulSerranoDev


#include "Player/GamePlayerState.h"
#include <AbilitySystem/GameAbilitySystemComponent.h>
#include <AbilitySystem/GameAttributeSet.h>

AGamePlayerState::AGamePlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UGameAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UGameAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AGamePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
