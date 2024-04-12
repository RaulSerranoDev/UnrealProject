// Copyright RaulSerranoDev


#include "AbilitySystem/Data/AttributeInfo.h"

FGameAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	FGameAttributeInfo GameAttributeInfo = FGameAttributeInfo();
	if (AttributesInformation.Contains(AttributeTag))
	{
		GameAttributeInfo.AttributeName = AttributesInformation[AttributeTag].AttributeName;
		GameAttributeInfo.AttributeDescription = AttributesInformation[AttributeTag].AttributeDescription;
		GameAttributeInfo.AttributeTag = AttributeTag;
		return GameAttributeInfo;
	}
	else if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return GameAttributeInfo;
}