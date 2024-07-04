// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/GameWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UGameWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "GAS|Points")
	FOnPlayerStatChangedSignature SpellPointsChangedDelegate;

};
