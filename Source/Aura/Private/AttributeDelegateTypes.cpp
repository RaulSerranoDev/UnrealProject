// Copyright RaulSerranoDev


#include "AttributeDelegateTypes.h"
#include "AbilitySystemComponent.h"

void UAttributeDelegateTypes::BindToAttributeValueChangeDelegate(UAbilitySystemComponent* ASC, const FGameplayAttribute& Attribute, FOnAttributeChangedSignature* AttributeChangeDelegate)
{
	ASC->GetGameplayAttributeValueChangeDelegate(
		Attribute).AddLambda(
			[AttributeChangeDelegate](const FOnAttributeChangeData& Data)
			{
				AttributeChangeDelegate->Broadcast(Data.NewValue);
			}
	);
}
