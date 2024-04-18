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
class UGameAbilitySystemComponent;
class USplineComponent;

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
	inline FHitResult GetCursorHit() const { return CursorHit; };

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void CursorTrace();
	void AutoRun();

	void Move(const FInputActionValue& InputActionValue);
	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };

	void AbilityInputTagPressed(const FInputActionValue& Value, FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(const FInputActionInstance& Instance, FGameplayTag InputTag);

	UGameAbilitySystemComponent* GetASC();

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> GameContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	TObjectPtr<UGameAbilitySystemComponent> GameAbilitySystemComponent;

	TScriptInterface<IHighlightInterface> LastActor;
	TScriptInterface<IHighlightInterface> CurrentActor;
	FHitResult CursorHit;

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	bool bShiftKeyDown = false;
};
