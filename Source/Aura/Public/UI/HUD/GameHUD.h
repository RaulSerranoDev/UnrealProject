// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

class UGameUserWidget;

/**
 *
 */
UCLASS()
class AURA_API AGameHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	TObjectPtr <UGameUserWidget> OverlayWidget;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf <UGameUserWidget> OverlayWidgetClass;
};
