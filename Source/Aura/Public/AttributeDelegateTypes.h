// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/GameWidgetController.h"
#include "AttributeDelegateTypes.generated.h"

struct FGameplayAttribute;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

/**
 *
 */
UCLASS()
class AURA_API UAttributeDelegateTypes : public UObject
{
	GENERATED_BODY()

public:
	static void BindToAttributeValueChangeDelegate(UAbilitySystemComponent* ASC, const FGameplayAttribute& Attribute, FOnAttributeChangedSignature* AttributeChangeDelegate);

};
