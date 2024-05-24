// Copyright RaulSerranoDev


#include "Character/PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "Player/GamePlayerState.h"
#include "Player/GamePlayerController.h"
#include "UI/HUD/GameHUD.h"
#include "AbilitySystem/Data/LevelUpInfo.h"

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

	CharacterClass = ECharacterClass::Elementalist;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

int32 APlayerCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const AGamePlayerState* GamePlayerState = GetPlayerState<AGamePlayerState>();
	check(GamePlayerState);
	return GamePlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 APlayerCharacter::GetXP_Implementation() const
{
	const AGamePlayerState* GamePlayerState = GetPlayerState<AGamePlayerState>();
	check(GamePlayerState);
	return GamePlayerState->GetXP();
}

int32 APlayerCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	const AGamePlayerState* GamePlayerState = GetPlayerState<AGamePlayerState>();
	check(GamePlayerState);
	return GamePlayerState->LevelUpInfo->LevelUpInfo[Level].AttributePointReward;
}

int32 APlayerCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	const AGamePlayerState* GamePlayerState = GetPlayerState<AGamePlayerState>();
	check(GamePlayerState);
	return GamePlayerState->LevelUpInfo->LevelUpInfo[Level].SpellPointReward;
}

void APlayerCharacter::AddToXP_Implementation(int32 InXP)
{
	AGamePlayerState* GamePlayerState = GetPlayerState<AGamePlayerState>();
	check(GamePlayerState);
	GamePlayerState->AddToXP(InXP);
}

void APlayerCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AGamePlayerState* GamePlayerState = GetPlayerState<AGamePlayerState>();
	check(GamePlayerState);
	GamePlayerState->AddToLevel(InPlayerLevel);
}

void APlayerCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	// TODO: Add AttributePoints to PlayerState
}

void APlayerCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	// TODO: Add SpellPoints to PlayerState
}

void APlayerCharacter::LevelUp_Implementation()
{
}

int32 APlayerCharacter::GetPlayerLevel_Implementation() const
{
	const AGamePlayerState* GamePlayerState = GetPlayerState<AGamePlayerState>();
	check(GamePlayerState);
	return GamePlayerState->GetPlayerLevel();
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

	InitializeDefaultAttributes();
}

void APlayerCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}
