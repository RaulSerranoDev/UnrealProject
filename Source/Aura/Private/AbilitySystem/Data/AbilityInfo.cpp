// Copyright RaulSerranoDev


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/GameLogChannels.h"

FGameAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	FGameAbilityInfo GameAbilityInfo = FGameAbilityInfo();
	if (AbilitiesInfo.Contains(AbilityTag))
	{
		GameAbilityInfo = AbilitiesInfo[AbilityTag];
		GameAbilityInfo.AbilityTag = AbilityTag;
		return GameAbilityInfo;
	}
	else if (bLogNotFound)
	{
		UE_LOG(LogGame, Error, TEXT("Can't find Info for AbilityTag [%s] on AbilityInfo [%s]."), *AbilityTag.ToString(), *GetNameSafe(this));
	}

	return GameAbilityInfo;
}
