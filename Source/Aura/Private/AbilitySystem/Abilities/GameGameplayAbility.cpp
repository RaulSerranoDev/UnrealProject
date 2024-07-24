// Copyright RaulSerranoDev


#include "AbilitySystem/Abilities/GameGameplayAbility.h"

#include "AbilitySystem/GameAttributeSet.h"

FString UGameGameplayAbility::GetDescription_Implementation(int32 Level)
{
	return FString::Printf(TEXT("%s, <Level>%d</>"), L"Default Ability Name - LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum", Level);
}

FString UGameGameplayAbility::GetNextLevelDescription_Implementation(int32 Level)
{
	return FString::Printf(TEXT("Next Level: <Level>%d</>\nCauses much more damage "), Level);
}

FString UGameGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("Spell Locked Unil Level: <Level>%d</>"), Level);
}

int32 UGameGameplayAbility::GetManaCost(int32 Level) const
{
	float ManaCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UGameAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, ManaCost);
				break;
			}
		}
	}
	return FMath::Abs(FMath::RoundToInt(ManaCost));
}

float UGameGameplayAbility::GetCD(int32 Level) const
{
	float Cooldown = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(Level, Cooldown);
	}
	return Cooldown;
}
