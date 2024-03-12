// Copyright RaulSerranoDev


#include "Player/GamePlayerController.h"

#include "EnhancedInputSubsystems.h"

AGamePlayerController::AGamePlayerController()
{
	bReplicates = true;
}

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(GameContext);	// TODO: Comprobar

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(GameContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}
