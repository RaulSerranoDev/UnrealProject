// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class AURA_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGamePlayerController();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> GameContext;
};
