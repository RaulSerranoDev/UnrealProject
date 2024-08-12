// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoadScreenHUD.generated.h"

class ULoadScreenWidget;
class UMVVM_LoadScreen;

/**
 *
 */
UCLASS()
class AURA_API ALoadScreenHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget>LoadScreenWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadScreen> LoadScreenViewModelClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULoadScreenWidget> LoadScreenWidget;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_LoadScreen> LoadScreenViewModel;
};
