// Copyright RaulSerranoDev


#include "AbilitySystem/GameAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"
#include "UI/HUD/GameHUD.h"
#include "Player/GamePlayerState.h"
#include "UI/WidgetController/GameWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "Game/MainGameModeBase.h"
#include "AbilityTypes.h"
#include "Interaction/CombatInterface.h"

UOverlayWidgetController* UGameAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		if (AGameHUD* GameHUD = PC->GetHUD<AGameHUD>())
		{
			AGamePlayerState* PS = PC->GetPlayerState<AGamePlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return GameHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UGameAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		if (AGameHUD* GameHUD = PC->GetHUD<AGameHUD>())
		{
			AGamePlayerState* PS = PC->GetPlayerState<AGamePlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return GameHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
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
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());
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

void UGameAbilitySystemLibrary::GetLivePlayersWithinRadius(
	const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors,
	const TArray<AActor*>& ActorsToIgnore,
	float Radius,
	const FVector& SphereOrigin,
	FName FilterTag)
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
	}
}

void UGameAbilitySystemLibrary::ApplyGameplayEffectHelper(TSubclassOf<UGameplayEffect> GEClass, int Level, FGameplayEffectContextHandle ContextHandle, UAbilitySystemComponent* ASC)
{
	const FGameplayEffectSpecHandle AttributesSpecHandle = ASC->MakeOutgoingSpec(GEClass, Level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*AttributesSpecHandle.Data);
}
