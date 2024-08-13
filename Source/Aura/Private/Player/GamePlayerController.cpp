// Copyright RaulSerranoDev


#include "Player/GamePlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SplineComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"

#include "Input/GameInputComponent.h"
#include "Interaction/HighlightInterface.h"
#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "GameGameplayTags.h"
#include "Interaction/SelectableInterface.h"
#include "Aura/Aura.h"
#include "UI/Widget/DamageTextComponent.h"
#include "Actor/MagicCircle.h"
#include "Interaction/EnemyInterface.h"

AGamePlayerController::AGamePlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AGamePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	AutoRun();
	UpdateMagicCircleLocation();
}

void AGamePlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlocked, bool bCritical)
{
	if (!IsValid(TargetCharacter) || !DamageTextComponentClass || !IsLocalController()) return;

	UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
	DamageText->RegisterComponent();
	DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	DamageText->SetDamageText(DamageAmount, bBlocked, bCritical);
}

void AGamePlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	bShowMouseCursor = false;
	if (IsValid(MagicCircle)) return;

	MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass, CursorHit.ImpactPoint, FRotator::ZeroRotator);
	if (DecalMaterial)
	{
		MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
	}
}

void AGamePlayerController::HideMagicCircle()
{
	bShowMouseCursor = true;
	if (!IsValid(MagicCircle)) return;

	MagicCircle->Destroy();
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
	GameInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ThisClass::ShiftPressed);
	GameInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ThisClass::ShiftReleased);
	GameInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AGamePlayerController::CursorTrace()
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(TAG_Player_Block_CursorTrace))
	{
		UnHighlightActor(LastActor);
		UnHighlightActor(CurrentActor);
		LastActor = nullptr;
		CurrentActor = nullptr;
		return;
	}

	const ECollisionChannel TraceChannel = IsValid(MagicCircle) ? ECC_ExcludePlayers : ECC_Visibility;
	GetHitResultUnderCursor(TraceChannel, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = CurrentActor;
	if (IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UHighlightInterface>())
	{
		CurrentActor = CursorHit.GetActor();
	}
	else
	{
		CurrentActor = nullptr;
	}

	if (LastActor == CurrentActor) return;

	UnHighlightActor(LastActor);
	HighlightActor(CurrentActor);
}

void AGamePlayerController::AutoRun()
{
	if (!bAutoRunning) return;

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AGamePlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(TAG_Player_Block_InputPressed)) return;

	APawn* ControlledPawn = GetPawn<APawn>();
	if (!ControlledPawn)
		return;

	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	bAutoRunning = false;
	ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
	ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
}

void AGamePlayerController::AbilityInputTagPressed(const FInputActionValue& Value, FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(TAG_Player_Block_InputPressed)) return;

	if (InputTag.MatchesTagExact(TAG_InputTag_LMB))
	{
		if (IsValid(CurrentActor))
		{
			TargetingStatus = CurrentActor->Implements<UEnemyInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNonEnemy;
			bAutoRunning = false;
		}
		else
		{
			TargetingStatus = ETargetingStatus::NotTargeting;
		}
	}

	if (GetASC())
	{
		GetASC()->AbilityInputTagPressed(InputTag);
	}
}

void AGamePlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(TAG_Player_Block_InputReleased)) return;

	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);

	if (GetASC() && GetASC()->HasMatchingGameplayTag(TAG_Player_Block_InputReleasedMovement)) return;

	if (!InputTag.MatchesTagExact(TAG_InputTag_LMB) || TargetingStatus == ETargetingStatus::TargetingEnemy || bShiftKeyDown) return;

	const APawn* ControlledPawn = GetPawn();
	if (FollowTime < ShortPressThreshold && ControlledPawn)
	{
		if (IsValid(CurrentActor) && CurrentActor->Implements<UHighlightInterface>())
		{
			IHighlightInterface::Execute_SetMoveToLocation(CurrentActor, CachedDestination);
		}
		else
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
		}

		if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
		{
			Spline->ClearSplinePoints();
			for (const FVector& Point : NavPath->PathPoints)
			{
				Spline->AddSplinePoint(Point, ESplineCoordinateSpace::World);
			}
			if (!NavPath->PathPoints.IsEmpty())
			{
				CachedDestination = NavPath->PathPoints.Last();
				bAutoRunning = true;
			}
		}
	}

	FollowTime = 0.f;
	TargetingStatus = ETargetingStatus::NotTargeting;
}

void AGamePlayerController::AbilityInputTagHeld(const FInputActionInstance& Instance, FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(TAG_Player_Block_InputHeld)) return;

	if (!InputTag.MatchesTagExact(TAG_InputTag_LMB) || TargetingStatus == ETargetingStatus::TargetingEnemy || bShiftKeyDown)
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		FHitResult Hit;
		if (GetHitResultUnderCursor(ECC_Selection, false, Hit) && Hit.bBlockingHit)	CachedDestination = Hit.ImpactPoint;

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UGameAbilitySystemComponent* AGamePlayerController::GetASC()
{
	if (!GameAbilitySystemComponent)
	{
		GameAbilitySystemComponent = Cast<UGameAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return GameAbilitySystemComponent;
}

void AGamePlayerController::UpdateMagicCircleLocation()
{
	if (!IsValid(MagicCircle)) return;

	MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
}

void AGamePlayerController::HighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_HighlightActor(InActor);
	}
}

void AGamePlayerController::UnHighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_UnHighlightActor(InActor);
	}
}
