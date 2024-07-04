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
	DOREPLIFETIME(AGamePlayerState, AttributePoints);
	DOREPLIFETIME(AGamePlayerState, SpellPoints);
}

void AGamePlayerState::AddToXP(int32 InXP)
{
	XP += InXP;

	while (CanLevelUp(XP))
	{
		AddToLevel(1);

		if (UGameAbilitySystemComponent* ASC = Cast<UGameAbilitySystemComponent>(AbilitySystemComponent))
		{
			ASC->UpdateAbilityStatuses(Level);
		}

		const int32 AttributePointsReward = LevelUpInfo->LevelUpInfo[Level - 1].AttributePointReward;;
		const int32 SpellPointsReward = LevelUpInfo->LevelUpInfo[Level - 1].SpellPointReward;

		AddToAttributePoints(AttributePointsReward);
		AddToSpellPoints(SpellPointsReward);

		CastChecked<UGameAttributeSet>(AttributeSet)->RefillVitalAttributes();
	}

	OnXPChangedDelegate.Broadcast(XP);
}

void AGamePlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AGamePlayerState::AddToAttributePoints(int32 InAttributePoints)
{
	AttributePoints += InAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AGamePlayerState::AddToSpellPoints(int32 InSpellPoints)
{
	SpellPoints += InSpellPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AGamePlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AGamePlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

bool AGamePlayerState::CanLevelUp(int32 InXP) const
{
	const int32 NewLevel = LevelUpInfo->FindLevelForXP(InXP);
	return NewLevel > Level;
}

void AGamePlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void AGamePlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void AGamePlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AGamePlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}
