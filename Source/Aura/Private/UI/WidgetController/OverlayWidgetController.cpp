// Copyright RaulSerranoDev


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/GameAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UGameAttributeSet* GameAttributeSet = CastChecked<UGameAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(GameAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(GameAttributeSet->GetMaxHealth());
}
