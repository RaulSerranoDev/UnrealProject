// Copyright RaulSerranoDev


#include "UI/HUD/GameHUD.h"

#include "UI/Widget/GameUserWidget.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* Widget = CreateWidget<UGameUserWidget>(GetWorld(), OverlayWidgetClass);
	Widget->AddToViewport();
}
