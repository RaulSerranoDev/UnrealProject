// Copyright RaulSerranoDev


#include "Player/GamePlayerState.h"
#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "AbilitySystem/GameAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/Data/LevelUpInfo.h"

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
	DOREPLIFETIME(AGamePlayerState, XP);
}

void AGamePlayerState::AddToXP(int32 InXP)
{
	XP += InXP;

	while (CanLevelUp(XP))
	{
		AddToLevel(1);

		const int32 AttributePointsReward = LevelUpInfo->LevelUpInfo[Level - 1].AttributePointReward;;
		const int32 SpellPointsReward = LevelUpInfo->LevelUpInfo[Level - 1].SpellPointReward;

		// TODO: Add AttributePoints to PlayerState
		// AttributePointsReward

		// TODO: Add SpellPoints to PlayerState
		// SpellPointsReward

		CastChecked<UGameAttributeSet>(AttributeSet)->RefillVitalAttributes();
	}

	OnXPChangeDelegate.Broadcast(XP);
}

void AGamePlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangeDelegate.Broadcast(Level);
}

void AGamePlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangeDelegate.Broadcast(XP);
}

void AGamePlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangeDelegate.Broadcast(Level);
}

bool AGamePlayerState::CanLevelUp(int32 InXP) const
{
	const int32 NewLevel = LevelUpInfo->FindLevelForXP(InXP);
	return NewLevel > Level;
}

void AGamePlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangeDelegate.Broadcast(Level);
}

void AGamePlayerState::OnRep_XP(int32 OldLevel)
{
	OnXPChangeDelegate.Broadcast(XP);
}
