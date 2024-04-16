// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Input/InputConfig.h"
#include "GameInputComponent.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UGameInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
inline void UGameInputComponent::BindAbilityActions(const UInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (auto& Pair : InputConfig->AbilityInputActions)
	{
		FGameplayTag& Tag = Pair.Key;
		UInputAction& Action = Pair.Value;

		if (!Tag.IsValid() || !Action)
			continue;

		if (PressedFunc)
		{
			BindAction(Action, ETriggerEvent::Started, Object, PressedFunc, Tag);
		}
		if (ReleasedFunc)
		{
			BindAction(Action, ETriggerEvent::Canceled, Object, ReleasedFunc, Tag);
		}
		if (HeldFunc)
		{
			BindAction(Action, ETriggerEvent::Triggered, Object, HeldFunc, Tag);
		}
	}
}
