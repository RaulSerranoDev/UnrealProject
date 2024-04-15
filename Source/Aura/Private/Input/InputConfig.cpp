// Copyright RaulSerranoDev


#include "Input/InputConfig.h"

// TODO: Si surgen errores mirar comentario seccion 99
const UInputAction* UInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	if (AbilityInputActions.Contains(InputTag))
	{
		return AbilityInputActions[InputTag];
	}
	else if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
