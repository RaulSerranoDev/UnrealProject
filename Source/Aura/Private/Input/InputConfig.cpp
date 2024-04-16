// Copyright RaulSerranoDev


#include "Input/InputConfig.h"

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

#if WITH_EDITOR

#include "Misc/DataValidation.h"

EDataValidationResult UInputConfig::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);
	unsigned int i = 0;
	for (auto& Pair : AbilityInputActions)
	{
		if (!Pair.Key.IsValid())
		{
			Result = EDataValidationResult::Invalid;
			const FText ErrorMessage = FText::FromString(FString::Printf(TEXT("\n\nA InputTag at index [%i] is invalid!"
				"\nPlease set a valid tag or delete the index entry in the Ability Input Actions Map."), i));
			Context.AddError(ErrorMessage);
		}
		if (!Pair.Value)
		{
			Result = EDataValidationResult::Invalid;
			const FText ErrorMessage = FText::FromString(FString::Printf(TEXT("\n\nA InputAction at index [%i] is none but has never to be none!"
				"\nPlease set a valid class or delete the index entry in the Ability Input Actions Map."), i));
			Context.AddError(ErrorMessage);
		}
		i++;
	}
	return Result;
}

#endif	