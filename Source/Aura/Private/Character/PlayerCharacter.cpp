// Copyright RaulSerranoDev


#include "Character/PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "Player/GamePlayerState.h"
#include "Player/GamePlayerController.h"
#include "NiagaraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/HUD/GameHUD.h"

APlayerCharacter::APlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;

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

void APlayerCharacter::AddToXP_Implementation(int32 InXP)
{
	AGamePlayerState* GamePlayerState = GetPlayerState<AGamePlayerState>();
	check(GamePlayerState);
	GamePlayerState->AddToXP(InXP);
}

void APlayerCharacter::LevelUp_Implementation(int32 NumLevelUps)
{
	MulticastLevelUpParticles();
}

int32 APlayerCharacter::GetAttributePoints_Implementation() const
{
	AGamePlayerState* GamePlayerState = GetPlayerState<AGamePlayerState>();
	check(GamePlayerState);
	return GamePlayerState->GetAttributePoints();
}

int32 APlayerCharacter::GetSpellPoints_Implementation() const
{
	AGamePlayerState* GamePlayerState = GetPlayerState<AGamePlayerState>();
	check(GamePlayerState);
	return GamePlayerState->GetSpellPoints();
}

void APlayerCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AGamePlayerState* GamePlayerState = GetPlayerState<AGamePlayerState>();
	check(GamePlayerState);
	GamePlayerState->AddToAttributePoints(InAttributePoints);
}

void APlayerCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	AGamePlayerState* GamePlayerState = GetPlayerState<AGamePlayerState>();
	check(GamePlayerState);
	GamePlayerState->AddToSpellPoints(InSpellPoints);
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

	// Suscribe to the player state level up delegate
	GamePlayerState->OnLevelChangedDelegate.AddLambda([this](int32 NewLevel) {ThisClass::LevelUp_Implementation(NewLevel); });
}

void APlayerCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void APlayerCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (!IsValid(LevelUpNiagaraComponent)) return;

	const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
	const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
	const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();

	LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
	LevelUpNiagaraComponent->Activate(true);
}
