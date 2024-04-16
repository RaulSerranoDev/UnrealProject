// Copyright RaulSerranoDev


#include "Player/GamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Input/GameInputComponent.h"
#include "Interaction/HighlightInterface.h"

AGamePlayerController::AGamePlayerController()
{
	bReplicates = true;
}

void AGamePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(GameContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(GameContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UGameInputComponent* GameInputComponent = CastChecked<UGameInputComponent>(InputComponent);
	GameInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	GameInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AGamePlayerController::Move(const FInputActionValue& InputActionValue)
{
	APawn* ControlledPawn = GetPawn<APawn>();
	if (!ControlledPawn)
		return;

	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
	ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
}

void AGamePlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = CurrentActor;
	CurrentActor = CursorHit.GetActor();

	if (LastActor == CurrentActor) return;

	if (LastActor) LastActor->UnHighlightActor();
	if (CurrentActor) CurrentActor->HighlightActor();
}

void AGamePlayerController::AbilityInputTagPressed(const FInputActionValue& Value, FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void AGamePlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Blue, *InputTag.ToString());
}

void AGamePlayerController::AbilityInputTagHeld(const FInputActionInstance& Instance, FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, *InputTag.ToString());
}
