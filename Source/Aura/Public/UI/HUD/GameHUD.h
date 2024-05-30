// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/GameWidgetController.h"
#include "GameHUD.generated.h"

class UGameUserWidget;
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
class USpellMenuWidgetController;

/**
 *
 */
UCLASS()
class AURA_API AGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	template <typename T>
	T* CreateOrGetWidgetController(TObjectPtr<T>& WidgetController,
		TSubclassOf<UGameWidgetController> WidgetControllerClass,
		const FWidgetControllerParams& WCParams);

private:
	UPROPERTY()
	TObjectPtr <UGameUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf <UGameUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr <UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf <UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr <UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf <UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr <USpellMenuWidgetController> SpellMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf <USpellMenuWidgetController> SpellMenuWidgetControllerClass;

};

template<typename T>
inline T* AGameHUD::CreateOrGetWidgetController(TObjectPtr<T>& WidgetController,
	TSubclassOf<UGameWidgetController> WidgetControllerClass,
	const FWidgetControllerParams& WCParams)
{
	if (WidgetController == nullptr)
	{
		WidgetController = NewObject<T>(this, WidgetControllerClass);
		WidgetController->SetWidgetControllerParams(WCParams);
		WidgetController->BindCallbacksToDependencies();
	}

	if (!WidgetController->IsA<UGameWidgetController>())
	{
		UE_LOG(LogTemp, Error, TEXT("CreateOrGetWidgetController with a WidgetController that is not a UGameWidgetController"));
	}

	return WidgetController;
}
