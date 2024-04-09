// Copyright RaulSerranoDev


#include "Player/GamePlayerState.h"
#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "AbilitySystem/GameAttributeSet.h"
#include "Net/UnrealNetwork.h"

AGamePlayerState::AGamePlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UGameAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UGameAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AGamePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGamePlayerState, Level);
}

void AGamePlayerState::OnRep_Level(int32 OldLevel)
{
}
