// Copyright RaulSerranoDev


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/GameAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UGameAttributeSet* GameAttributeSet = CastChecked<UGameAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(GameAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(GameAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(GameAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(GameAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UGameAttributeSet* GameAttributeSet = CastChecked<UGameAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GameAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GameAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GameAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GameAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
