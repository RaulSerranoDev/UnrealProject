// Copyright RaulSerranoDev


#include "Character/PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

#include "AbilitySystem/GameAbilitySystemComponent.h"
#include "Player/GamePlayerState.h"
#include "Player/GamePlayerController.h"
#include "NiagaraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/HUD/GameHUD.h"
#include "GameGameplayTags.h"
#include "Game/MainGameModeBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "AbilitySystem/GameAttributeSet.h"
#include "AbilitySystem/GameAbilitySystemLibrary.h"

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
	LoadProgress();
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

void APlayerCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (AGamePlayerController* GamePlayerController = Cast<AGamePlayerController>(GetController()))
	{
		GamePlayerController->ShowMagicCircle(DecalMaterial);
	}
}

void APlayerCharacter::HideMagicCircle_Implementation()
{
	if (AGamePlayerController* GamePlayerController = Cast<AGamePlayerController>(GetController()))
	{
		GamePlayerController->HideMagicCircle();
	}
}

void APlayerCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	AMainGameModeBase* GameMode = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!GameMode) return;

	ULoadScreenSaveGame* SaveData = GameMode->RetrieveInGameSaveData();
	if (SaveData == nullptr) return;

	SaveData->PlayerStartTag = CheckpointTag;

	if (AGamePlayerState* GamePlayerState = Cast<AGamePlayerState>(GetPlayerState()))
	{
		SaveData->PlayerLevel = GamePlayerState->GetPlayerLevel();
		SaveData->XP = GamePlayerState->GetXP();
		SaveData->AttributePoints = GamePlayerState->GetAttributePoints();
		SaveData->SpellPoints = GamePlayerState->GetSpellPoints();
	}
	SaveData->Strength = UGameAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
	SaveData->Intelligence = UGameAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
	SaveData->Resilience = UGameAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
	SaveData->Vigor = UGameAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());

	SaveData->bFirstTimeLoadIn = false;
	GameMode->SaveInGameProgressData(SaveData);
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
	OnASCRegistered.Broadcast(AbilitySystemComponent);
	Cast<UGameAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AbilitySystemComponent->RegisterGameplayTagEvent(TAG_Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::StunTagChanged);

	if (AGamePlayerController* GamePlayerController = Cast<AGamePlayerController>(GetController()))
	{
		if (AGameHUD* GameHUD = Cast<AGameHUD>(GamePlayerController->GetHUD()))
		{
			GameHUD->InitOverlay(GamePlayerController, GamePlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	// Suscribe to the player state level up delegate
	GamePlayerState->OnLevelChangedDelegate.AddLambda([this](int32 NewLevel, bool bLevelUp) {if (bLevelUp)ThisClass::LevelUp_Implementation(NewLevel); });
}

void APlayerCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void APlayerCharacter::OnRep_Stunned()
{
	if (UGameAbilitySystemComponent* ASC = Cast<UGameAbilitySystemComponent>(AbilitySystemComponent))
	{
		FGameplayTagContainer BlockedTags = FGameplayTagContainer();
		BlockedTags.AddTag(TAG_Player_Block_CursorTrace);
		BlockedTags.AddTag(TAG_Player_Block_InputHeld);
		BlockedTags.AddTag(TAG_Player_Block_InputPressed);
		BlockedTags.AddTag(TAG_Player_Block_InputReleased);
		if (bIsStunned)
		{
			ASC->AddLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Activate();
		}
		else
		{
			ASC->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Deactivate();
		}
	}
}

void APlayerCharacter::OnRep_Burned()
{
	if (bIsBurned)
	{
		BurnDebuffComponent->Activate();
	}
	else
	{
		BurnDebuffComponent->Deactivate();
	}
}

void APlayerCharacter::LoadProgress()
{
	AMainGameModeBase* MainGameMode = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!MainGameMode) return;

	ULoadScreenSaveGame* SaveData = MainGameMode->RetrieveInGameSaveData();
	if (SaveData == nullptr) return;

	if (SaveData->bFirstTimeLoadIn)
	{
		InitializeDefaultAttributes();
		AddCharacterAbilities();
	}
	else
	{
		//TODO: Load in Abilities from disk

		if (AGamePlayerState* GamePlayerState = Cast<AGamePlayerState>(GetPlayerState()))
		{
			GamePlayerState->SetLevel(SaveData->PlayerLevel);
			GamePlayerState->SetXP(SaveData->XP);
			GamePlayerState->SetAttributePoints(SaveData->AttributePoints);
			GamePlayerState->SetSpellPoints(SaveData->SpellPoints);
		}

		UGameAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveData);
	}
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
