// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputConfig.h"
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
		if (!Pair.Key.IsValid() || !Pair.Value)
			continue;

		if (PressedFunc)
		{
			BindAction(Pair.Value, ETriggerEvent::Started, Object, PressedFunc, Pair.Key);
		}
		if (ReleasedFunc)
		{
			BindAction(Pair.Value, ETriggerEvent::Completed, Object, ReleasedFunc, Pair.Key);
		}
		if (HeldFunc)
		{
			BindAction(Pair.Value, ETriggerEvent::Triggered, Object, HeldFunc, Pair.Key);
		}
	}
}
