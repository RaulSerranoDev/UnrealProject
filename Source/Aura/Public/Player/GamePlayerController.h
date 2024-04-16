// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "GamePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IHighlightInterface;
class UInputConfig;
struct FInputActionInstance;

/**
 *
 */
UCLASS()
class AURA_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AGamePlayerController();

	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();

	void AbilityInputTagPressed(const FInputActionValue& Value, FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(const FInputActionInstance& Instance, FGameplayTag InputTag);

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> GameContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputConfig> InputConfig;

	TScriptInterface<IHighlightInterface> LastActor;
	TScriptInterface<IHighlightInterface> CurrentActor;
};
