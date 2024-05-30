// Copyright RaulSerranoDev


#include "AbilitySystem/GameAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"
#include "DrawDebugHelpers.h"

#include "UI/HUD/GameHUD.h"
#include "Player/GamePlayerState.h"
#include "UI/WidgetController/GameWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "Game/MainGameModeBase.h"
#include "AbilityTypes.h"
#include "Interaction/CombatInterface.h"

bool UGameAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AGameHUD*& OutHUD)
{
	if (APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		OutHUD = PC->GetHUD<AGameHUD>();
		if (OutHUD)
		{
			AGamePlayerState* PS = PC->GetPlayerState<AGamePlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			OutWCParams.PlayerController = PC;
			OutWCParams.PlayerState = PS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.AttributeSet = AS;

			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UGameAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AGameHUD* HUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, HUD))
	{
		return HUD->GetOverlayWidgetController(WCParams);
	}

	return nullptr;
}

UAttributeMenuWidgetController* UGameAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AGameHUD* HUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, HUD))
	{
		return HUD->GetAttributeMenuWidgetController(WCParams);
	}

	return nullptr;
}

USpellMenuWidgetController* UGameAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AGameHUD* HUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, HUD))
	{
		return HUD->GetSpellMenuWidgetController(WCParams);
	}

	return nullptr;
}

void UGameAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	if (const UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		const AActor* AvatarActor = ASC->GetAvatarActor();

		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		ContextHandle.AddSourceObject(AvatarActor);

		FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
		ApplyGameplayEffectHelper(ClassDefaultInfo.PrimaryAttributes, Level, ContextHandle, ASC);

		ApplyGameplayEffectHelper(CharacterClassInfo->SecondaryAttributes, Level, ContextHandle, ASC);
		ApplyGameplayEffectHelper(CharacterClassInfo->VitalAttributes, Level, ContextHandle, ASC);
	}
}

void UGameAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	if (UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
			ASC->GiveAbility(AbilitySpec);
		}
		const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
		for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
		{
			if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
				ASC->GiveAbility(AbilitySpec);
			}
		}
	}
}

UCharacterClassInfo* UGameAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AMainGameModeBase* GM = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	return GM ? GM->CharacterClassInfo : nullptr;
}

bool UGameAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameGameplayEffectContext* GameContext = static_cast<const FGameGameplayEffectContext*>(EffectContextHandle.Get());
	return GameContext ? GameContext->IsBlockedHit() : false;
}

bool UGameAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameGameplayEffectContext* GameContext = static_cast<const FGameGameplayEffectContext*>(EffectContextHandle.Get());
	return GameContext ? GameContext->IsCriticalHit() : false;
}

void UGameAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bBlocked)
{
	if (FGameGameplayEffectContext* GameContext = static_cast<FGameGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		GameContext->SetIsBlockedHit(bBlocked);
	}
}

void UGameAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bCritical)
{
	if (FGameGameplayEffectContext* GameContext = static_cast<FGameGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		GameContext->SetIsCriticalHit(bCritical);
	}
}

TArray<FGameplayTag> UGameAbilitySystemLibrary::CallerMagnitudeTags(TSubclassOf<UGameplayEffect> GameplayEffect)
{
	UGameplayEffect* GE = GameplayEffect.GetDefaultObject();

	TArray<FGameplayTag> CallerTags;
	for (FGameplayModifierInfo Info : GE->Modifiers)
	{
		if (Info.ModifierMagnitude.GetMagnitudeCalculationType() == EGameplayEffectMagnitudeCalculation::SetByCaller)
		{
			CallerTags.Add(Info.ModifierMagnitude.GetSetByCallerFloat().DataTag);
		}
	}

	return CallerTags;
}

void UGameAbilitySystemLibrary::GetLivePlayersWithinRadius(
	const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors,
	const TArray<AActor*>& ActorsToIgnore,
	float Radius,
	const FVector& SphereOrigin,
	FName FilterTag,
	bool bShowDebugSphere)
{
	FCollisionQueryParams SphereParams;
	SphereParams.MobilityType = EQueryMobilityType::Dynamic;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	// query scene to see what we hit
	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if ((FilterTag.IsNone() || Overlap.GetActor()->ActorHasTag(FilterTag))
				&& Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(Overlap.GetActor());
			}
		}

		if (bShowDebugSphere)
		{
			DrawDebugSphere(World, SphereOrigin, Radius, 12, FColor::Magenta, false, 0.5f);
		}
	}
}

bool UGameAbilitySystemLibrary::IsOnSameTeam(const AActor* FirstActor, const AActor* SecondActor)
{
	if (!FirstActor || !SecondActor)
		return true;

	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bSameTeam = bBothArePlayers || bBothAreEnemies;
	return bSameTeam;
}

int32 UGameAbilitySystemLibrary::GetXPReward(const UObject* WorldContextObject, ECharacterClass CharacterClass, const int32& Level)
{
	if (UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
		return static_cast<int32>(DefaultInfo.XPReward.GetValueAtLevel(Level));
	}
	return 0;
}

void UGameAbilitySystemLibrary::ApplyGameplayEffectHelper(TSubclassOf<UGameplayEffect> GEClass, int Level, FGameplayEffectContextHandle ContextHandle, UAbilitySystemComponent* ASC)
{
	const FGameplayEffectSpecHandle AttributesSpecHandle = ASC->MakeOutgoingSpec(GEClass, Level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*AttributesSpecHandle.Data);
}
