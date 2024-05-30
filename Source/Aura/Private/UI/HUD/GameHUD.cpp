// Copyright RaulSerranoDev


#include "UI/HUD/GameHUD.h"
#include "UI/Widget/GameUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"

UOverlayWidgetController* AGameHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	return CreateOrGetWidgetController<UOverlayWidgetController>(OverlayWidgetController, OverlayWidgetControllerClass, WCParams);
}

UAttributeMenuWidgetController* AGameHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	return CreateOrGetWidgetController<UAttributeMenuWidgetController>(AttributeMenuWidgetController, AttributeMenuWidgetControllerClass, WCParams);
}

USpellMenuWidgetController* AGameHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	return CreateOrGetWidgetController<USpellMenuWidgetController>(SpellMenuWidgetController, SpellMenuWidgetControllerClass, WCParams);
}

void AGameHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass unitialized, please fill out BP_GameHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass unitialized, please fill out BP_GameHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UGameUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();

	Widget->AddToViewport();
}
