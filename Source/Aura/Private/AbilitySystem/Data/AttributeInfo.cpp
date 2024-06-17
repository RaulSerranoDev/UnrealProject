// Copyright RaulSerranoDev


#include "AbilitySystem/Data/AttributeInfo.h"

#include "Aura/GameLogChannels.h"

FGameAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	FGameAttributeInfo GameAttributeInfo = FGameAttributeInfo();
	if (AttributesInformation.Contains(AttributeTag))
	{
		GameAttributeInfo = AttributesInformation[AttributeTag];
		GameAttributeInfo.AttributeTag = AttributeTag;
		return GameAttributeInfo;
	}
	else if (bLogNotFound)
	{
		UE_LOG(LogGame, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return GameAttributeInfo;
}
