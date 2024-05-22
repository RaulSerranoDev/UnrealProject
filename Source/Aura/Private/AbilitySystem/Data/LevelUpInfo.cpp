// Copyright RaulSerranoDev


#include "AbilitySystem/Data/LevelUpInfo.h"

#include "Aura/GameLogChannels.h"

int32 ULevelUpInfo::FindLevelForXP(const int32& XP, bool bLogNotFound) const
{
	for (size_t i = LevelUpInfo.Num() - 1; i >= 0; i--)
	{
		if (XP >= LevelUpInfo[i].XPRequirement)
			return i + 1;
	}

	if (bLogNotFound)
	{
		UE_LOG(LogGame, Error, TEXT("Can't find Info for XP [%s] on LevelUpInfo [%s]."), XP, *GetNameSafe(this));
	}

	return 0;
}
