// Copyright RaulSerranoDev


#include "UI/WidgetController/GameWidgetController.h"

#include "Player/GamePlayerController.h"
#include "Player/GamePlayerState.h"
#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "AbilitySystem/GameAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void UGameWidgetController::SetWidgetControllerParams(const FWidgetControllerParams WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UGameWidgetController::BroadcastInitialValues()
{
}

void UGameWidgetController::BindCallbacksToDependencies()
{
}

void UGameWidgetController::BroadcastAbilityInfo()
{
	if (!GetASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
		{
			FGameAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(UGameAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = UGameAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
			Info.StatusTag = UGameAbilitySystemComponent::GetStatusFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		});

	GetASC()->ForEachAbility(BroadcastDelegate);
}

AGamePlayerController* UGameWidgetController::GetPC()
{
	if (!GamePlayerController)
	{
		GamePlayerController = Cast<AGamePlayerController>(PlayerController);
	}
	return GamePlayerController;
}

AGamePlayerState* UGameWidgetController::GetPS()
{
	if (!GamePlayerState)
	{
		GamePlayerState = Cast<AGamePlayerState>(PlayerState);
	}
	return GamePlayerState;
}

UGameAbilitySystemComponent* UGameWidgetController::GetASC()
{
	if (!GameAbilitySystemComponent)
	{
		GameAbilitySystemComponent = Cast<UGameAbilitySystemComponent>(AbilitySystemComponent);
	}
	return GameAbilitySystemComponent;
}

UGameAttributeSet* UGameWidgetController::GetAS()
{
	if (!GameAttributeSet)
	{
		GameAttributeSet = Cast<UGameAttributeSet>(AttributeSet);
	}
	return GameAttributeSet;
}
