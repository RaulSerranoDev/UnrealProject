// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

class UGameUserWidget;
class UOverlayWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
struct FWidgetControllerParams;

/**
 *
 */
UCLASS()
class AURA_API AGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

public:
	UPROPERTY()
	TObjectPtr <UGameUserWidget> OverlayWidget;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf <UGameUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr <UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf <UOverlayWidgetController> OverlayWidgetControllerClass;
};
