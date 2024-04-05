// Copyright RaulSerranoDev


#include "Character/PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "Player/GamePlayerState.h"
#include "Player/GamePlayerController.h"
#include "UI/HUD/GameHUD.h"

APlayerCharacter::APlayerCharacter()
{
	UCharacterMovementComponent* PlayerCharacterMovement = GetCharacterMovement();
	PlayerCharacterMovement->bOrientRotationToMovement = true;
	PlayerCharacterMovement->RotationRate = FRotator(0.f, 400.f, 0.f);
	PlayerCharacterMovement->bConstrainToPlane = true;
	PlayerCharacterMovement->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

void APlayerCharacter::InitAbilityActorInfo()
{
	AGamePlayerState* GamePlayerState = GetPlayerState<AGamePlayerState>();
	check(GamePlayerState);
	AbilitySystemComponent = GamePlayerState->GetAbilitySystemComponent();
	AttributeSet = GamePlayerState->GetAttributeSet();
	AbilitySystemComponent->InitAbilityActorInfo(GamePlayerState, this);
	Cast<UGameAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (AGamePlayerController* GamePlayerController = Cast<AGamePlayerController>(GetController()))
	{
		if (AGameHUD* GameHUD = Cast<AGameHUD>(GamePlayerController->GetHUD()))
		{
			GameHUD->InitOverlay(GamePlayerController, GamePlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	InitializePrimaryAttributes();
}
