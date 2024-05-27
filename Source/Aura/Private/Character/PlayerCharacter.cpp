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

void APlayerCharacter::AddToXP_Implementation(int32 InXP)
{
	AGamePlayerState* GamePlayerState = GetPlayerState<AGamePlayerState>();
	check(GamePlayerState);
	GamePlayerState->AddToXP(InXP);
}

void APlayerCharacter::LevelUp_Implementation(int32 NumLevelUps)
{
	AGamePlayerState* GamePlayerState = GetPlayerState<AGamePlayerState>();
	check(GamePlayerState);

	GamePlayerState->AddToLevel(NumLevelUps);
	const int32 Level = GamePlayerState->GetPlayerLevel();

	int32 AttributePointsReward = 0;
	int32 SpellPointsReward = 0;
	for (int32 i = 0; i < NumLevelUps; i++)
	{
		AttributePointsReward += GamePlayerState->LevelUpInfo->LevelUpInfo[Level + 1 + i].AttributePointReward;
		SpellPointsReward += GamePlayerState->LevelUpInfo->LevelUpInfo[Level + 1 + i].SpellPointReward;
	}

	// TODO: Add AttributePoints to PlayerState
	// AttributePointsReward

	// TODO: Add SpellPoints to PlayerState
	// SpellPointsReward
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
