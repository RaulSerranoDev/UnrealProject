// Copyright RaulSerranoDev


#include "AbilitySystem/Abilities/GameGameplayAbility.h"

FString UGameGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("%s, <Level>%d</>"), L"Default Ability Name - LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum", Level);
}

FString UGameGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("Next Level: <Level>%d</>\nCauses much more damage "), Level);
}

FString UGameGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("Spell Locked Unil Level: <Level>%d</>"), Level);
}
