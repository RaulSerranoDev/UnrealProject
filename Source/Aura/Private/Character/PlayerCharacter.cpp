// Copyright RaulSerranoDev


#include "Character/PlayerCharacter.h"

#include <GameFramework/CharacterMovementComponent.h>

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
