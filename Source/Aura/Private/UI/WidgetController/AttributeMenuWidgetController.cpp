// Copyright RaulSerranoDev


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/GameAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "GameGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);

	UGameAttributeSet* AS = CastChecked<UGameAttributeSet>(AttributeSet);
	FGameAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(TAG_Attributes_Primary_Strength);
	Info.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
}
