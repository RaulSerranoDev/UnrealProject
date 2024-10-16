// Copyright RaulSerranoDev


#include "AbilitySystem/GameAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "UI/HUD/GameHUD.h"
#include "Player/GamePlayerState.h"
#include "UI/WidgetController/GameWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "Game/MainGameModeBase.h"
#include "AbilityTypes.h"
#include "Interaction/CombatInterface.h"
#include "GameGameplayTags.h"
#include "Game/LoadScreenSaveGame.h"
#include "AbilitySystem/GameAbilitySystemComponent.h"

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

void UGameAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SaveGame)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;

	const AActor* SourceAvatarActor = ASC->GetAvatarActor();

	FGameplayEffectContextHandle EffectContexthandle = ASC->MakeEffectContext();
	EffectContexthandle.AddSourceObject(SourceAvatarActor);

	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->PrimaryAttributes_SetByCaller, 1.f, EffectContexthandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, TAG_Attributes_Primary_Strength, SaveGame->Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, TAG_Attributes_Primary_Intelligence, SaveGame->Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, TAG_Attributes_Primary_Resilience, SaveGame->Resilience);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, TAG_Attributes_Primary_Vigor, SaveGame->Vigor);

	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, 1.f, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, 1.f, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
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

UAbilityInfo* UGameAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	AMainGameModeBase* GM = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	return GM ? GM->AbilityInfo : nullptr;
}

ULootTiers* UGameAbilitySystemLibrary::GetLootTiers(const UObject* WorldContextObject)
{
	AMainGameModeBase* GM = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	return GM ? GM->LootTiers : nullptr;
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

bool UGameAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameGameplayEffectContext* GameContext = static_cast<const FGameGameplayEffectContext*>(EffectContextHandle.Get());
	return GameContext ? GameContext->IsSuccessfulDebuff() : false;
}

float UGameAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameGameplayEffectContext* GameContext = static_cast<const FGameGameplayEffectContext*>(EffectContextHandle.Get());
	return GameContext ? GameContext->GetDebuffDamage() : 0.f;
}

float UGameAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameGameplayEffectContext* GameContext = static_cast<const FGameGameplayEffectContext*>(EffectContextHandle.Get());
	return GameContext ? GameContext->GetDebuffDuration() : 0.f;
}

float UGameAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameGameplayEffectContext* GameContext = static_cast<const FGameGameplayEffectContext*>(EffectContextHandle.Get());
	return GameContext ? GameContext->GetDebuffFrequency() : 0.f;
}

FGameplayTag UGameAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameGameplayEffectContext* GameContext = static_cast<const FGameGameplayEffectContext*>(EffectContextHandle.Get());
	return GameContext && GameContext->GetDamageType().IsValid() ? *GameContext->GetDamageType() : FGameplayTag();
}

FVector UGameAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameGameplayEffectContext* GameContext = static_cast<const FGameGameplayEffectContext*>(EffectContextHandle.Get());
	return GameContext ? GameContext->GetDeathImpulse() : FVector::ZeroVector;
}

FVector UGameAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameGameplayEffectContext* GameContext = static_cast<const FGameGameplayEffectContext*>(EffectContextHandle.Get());
	return GameContext ? GameContext->GetKnockbackForce() : FVector::ZeroVector;
}

bool UGameAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameGameplayEffectContext* GameContext = static_cast<const FGameGameplayEffectContext*>(EffectContextHandle.Get());
	return GameContext ? GameContext->IsRadialDamage() : false;
}

float UGameAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameGameplayEffectContext* GameContext = static_cast<const FGameGameplayEffectContext*>(EffectContextHandle.Get());
	return GameContext ? GameContext->GetRadialDamageInnerRadius() : 0.f;
}

float UGameAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameGameplayEffectContext* GameContext = static_cast<const FGameGameplayEffectContext*>(EffectContextHandle.Get());
	return GameContext ? GameContext->GetRadialDamageOuterRadius() : 0.f;
}

FVector UGameAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameGameplayEffectContext* GameContext = static_cast<const FGameGameplayEffectContext*>(EffectContextHandle.Get());
	return GameContext ? GameContext->GetRadialDamageOrigin() : FVector::ZeroVector;
}

float UGameAbilitySystemLibrary::GetRadialDamageMinPercentage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameGameplayEffectContext* GameContext = static_cast<const FGameGameplayEffectContext*>(EffectContextHandle.Get());
	return GameContext ? GameContext->GetRadialDamageMinPercentage() : 0.f;
}

bool UGameAbilitySystemLibrary::IsCharm(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameGameplayEffectContext* GameContext = static_cast<const FGameGameplayEffectContext*>(EffectContextHandle.Get());
	return GameContext ? GameContext->IsCharm() : false;
}

bool UGameAbilitySystemLibrary::ShouldHitReact(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameGameplayEffectContext* GameContext = static_cast<const FGameGameplayEffectContext*>(EffectContextHandle.Get());
	return GameContext ? GameContext->ShouldHitReact() : false;
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

void UGameAbilitySystemLibrary::SetIsSuccessfulDebuff(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bSuccessfulDebuff)
{
	if (FGameGameplayEffectContext* GameContext = static_cast<FGameGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		GameContext->SetIsSuccessfulDebuff(bSuccessfulDebuff);
	}
}

void UGameAbilitySystemLibrary::SetDebuffDamage(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float DebuffDamage)
{
	if (FGameGameplayEffectContext* GameContext = static_cast<FGameGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		GameContext->SetDebuffDamage(DebuffDamage);
	}
}

void UGameAbilitySystemLibrary::SetDebuffDuration(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float DebuffDuration)
{
	if (FGameGameplayEffectContext* GameContext = static_cast<FGameGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		GameContext->SetDebuffDuration(DebuffDuration);
	}
}

void UGameAbilitySystemLibrary::SetDebuffFrequency(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float DebuffFrequency)
{
	if (FGameGameplayEffectContext* GameContext = static_cast<FGameGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		GameContext->SetDebuffFrequency(DebuffFrequency);
	}
}

void UGameAbilitySystemLibrary::SetDamageType(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& DamageType)
{
	if (FGameGameplayEffectContext* GameContext = static_cast<FGameGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageTypePtr = MakeShared<FGameplayTag>(DamageType);
		GameContext->SetDamageType(DamageTypePtr);
	}
}

void UGameAbilitySystemLibrary::SetDeathImpulse(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& DeathImpulse)
{
	if (FGameGameplayEffectContext* GameContext = static_cast<FGameGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		GameContext->SetDeathImpulse(DeathImpulse);
	}
}

void UGameAbilitySystemLibrary::SetShouldHitReact(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool ShouldHitReact)
{
	if (FGameGameplayEffectContext* GameContext = static_cast<FGameGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		GameContext->SetShouldHitReact(ShouldHitReact);
	}
}

void UGameAbilitySystemLibrary::SetKnockbackForce(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& KnockbackForce)
{
	if (FGameGameplayEffectContext* GameContext = static_cast<FGameGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		GameContext->SetKnockbackForce(KnockbackForce);
	}
}

void UGameAbilitySystemLibrary::SetIsRadialDamage(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage)
{
	if (FGameGameplayEffectContext* GameContext = static_cast<FGameGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		GameContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UGameAbilitySystemLibrary::SetRadialDamageInnerRadius(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius)
{
	if (FGameGameplayEffectContext* GameContext = static_cast<FGameGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		GameContext->SetRadialDamageInnerRadius(InInnerRadius);
	}
}

void UGameAbilitySystemLibrary::SetRadialDamageOuterRadius(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius)
{
	if (FGameGameplayEffectContext* GameContext = static_cast<FGameGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		GameContext->SetRadialDamageOuterRadius(InOuterRadius);
	}
}

void UGameAbilitySystemLibrary::SetRadialDamageOrigin(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InOrigin)
{
	if (FGameGameplayEffectContext* GameContext = static_cast<FGameGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		GameContext->SetRadialDamageOrigin(InOrigin);
	}
}

void UGameAbilitySystemLibrary::SetRadialDamageMinPercentage(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InRadialDamageMinPercentage)
{
	if (FGameGameplayEffectContext* GameContext = static_cast<FGameGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		GameContext->SetRadialDamageMinPercentage(InRadialDamageMinPercentage);
	}
}

void UGameAbilitySystemLibrary::SetIsCharm(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bIsCharm)
{
	if (FGameGameplayEffectContext* GameContext = static_cast<FGameGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		GameContext->SetIsCharm(bIsCharm);
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

void UGameAbilitySystemLibrary::GetClosestTargets(const FVector& Origin, const int32& MaxTargets,
	const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets)
{
	if (Actors.IsEmpty()) return;
	TArray<AActor*> ActorsCopy = Actors;
	Algo::Sort(ActorsCopy, [Origin](const AActor* A, const AActor* B)
		{
			float DistanceA = FVector::DistSquared(A->GetActorLocation(), Origin);
			float DistanceB = FVector::DistSquared(B->GetActorLocation(), Origin);
			return DistanceA < DistanceB;
		});
	int32 Size = FMath::Min(MaxTargets, ActorsCopy.Num());
	for (int i = 0; i < Size; i++)
	{
		OutClosestTargets.Add(ActorsCopy[i]);
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

FGameplayEffectContextHandle UGameAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(DamageEffectParams.SourceASC->GetAvatarActor());
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce);
	SetIsCharm(EffectContextHandle, DamageEffectParams.bIsCharm);

	if (DamageEffectParams.bIsRadialDamage)
	{
		SetIsRadialDamage(EffectContextHandle, DamageEffectParams.bIsRadialDamage);
		SetRadialDamageInnerRadius(EffectContextHandle, DamageEffectParams.RadialDamageInnerRadius);
		SetRadialDamageOuterRadius(EffectContextHandle, DamageEffectParams.RadialDamageOuterRadius);
		SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.RadialDamageOrigin);
		SetRadialDamageMinPercentage(EffectContextHandle, DamageEffectParams.RadialDamageMinPercentage);
	}

	for (const TTuple<FGameplayTag, FDamageEffectType>& Pair : DamageEffectParams.DamageTypes)
	{
		const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceASC->MakeOutgoingSpec(DamageEffectParams.DamageEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, Pair.Value.Damage);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, TAG_Debuff_Chance, Pair.Value.DebuffChance);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, TAG_Debuff_Damage, Pair.Value.DebuffDamage);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, TAG_Debuff_Duration, Pair.Value.DebuffDuration);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, TAG_Debuff_Frequency, Pair.Value.DebuffFrequency);
		DamageEffectParams.TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	}

	return EffectContextHandle;
}

TArray<FRotator> UGameAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	if (NumRotators <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("NumRotators <= 0"));
		return TArray<FRotator>();
	}

	TArray<FRotator> Rotators;
	if (NumRotators == 1)
	{
		Rotators.Add(Forward.Rotation());
		return Rotators;
	}

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	const float DeltaSpread = Spread / NumRotators;
	for (int32 i = 0; i < NumRotators; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i + DeltaSpread / 2.f, FVector::UpVector);
		Rotators.Add(Direction.Rotation());
	}

	return Rotators;
}

TArray<FVector> UGameAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	if (NumVectors <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("NumRotators <= 0"));
		return TArray<FVector>();
	}

	TArray<FVector> Vectors;
	if (NumVectors == 1)
	{
		Vectors.Add(Forward);
		return Vectors;
	}

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	const float DeltaSpread = Spread / NumVectors;
	for (int32 i = 0; i < NumVectors; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i + DeltaSpread / 2.f, FVector::UpVector);
		Vectors.Add(Direction);
	}

	return Vectors;
}

float UGameAbilitySystemLibrary::GetRadialDamageWithFalloff(const AActor* TargetActor, float BaseDamage, float MinimumDamage, const FVector& Origin, float DamageInnerRadius, float DamageOuterRadius, float DamageFalloff)
{
	if (!TargetActor) return 0.f;

	FRadialDamageParams RadialDamageParams;
	RadialDamageParams.BaseDamage = BaseDamage;
	RadialDamageParams.DamageFalloff = DamageFalloff;
	RadialDamageParams.InnerRadius = DamageInnerRadius;
	RadialDamageParams.OuterRadius = DamageOuterRadius;
	RadialDamageParams.MinimumDamage = MinimumDamage;
	float DamageScale = RadialDamageParams.GetDamageScale((Origin - TargetActor->GetActorLocation()).Length());
	return BaseDamage * DamageScale;
}

int UGameAbilitySystemLibrary::GetLevelFromAbilityTag(const FGameplayTag& AbilityTag, UAbilitySystemComponent* ASC)
{
	if (UGameAbilitySystemComponent* GASC = Cast<UGameAbilitySystemComponent>(ASC))
	{
		return GASC->GetSpecFromAbilityTag(AbilityTag)->Level;
	}
	return 0;
}

void UGameAbilitySystemLibrary::SetIsRadialDamageEffectParam(UPARAM(ref)FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin, float RadialDamageMinPercentage)
{
	DamageEffectParams.bIsRadialDamage = bIsRadial;
	DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
	DamageEffectParams.RadialDamageOrigin = Origin;
	DamageEffectParams.RadialDamageMinPercentage = RadialDamageMinPercentage;
}

void UGameAbilitySystemLibrary::SetKnockbackDirection(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude)
{
	KnockbackDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
	}
	else
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * Magnitude;
	}
}

void UGameAbilitySystemLibrary::SetDeathImpulseDirection(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude)
{
	ImpulseDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * DamageEffectParams.DeathImpulseMagnitude;
	}
	else
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * Magnitude;
	}
}

void UGameAbilitySystemLibrary::SetTargetEffectParamsASC(UPARAM(ref)FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC)
{
	DamageEffectParams.TargetASC = InASC;
}

void UGameAbilitySystemLibrary::ApplyGameplayEffectHelper(TSubclassOf<UGameplayEffect> GEClass, int Level, FGameplayEffectContextHandle ContextHandle, UAbilitySystemComponent* ASC)
{
	const FGameplayEffectSpecHandle AttributesSpecHandle = ASC->MakeOutgoingSpec(GEClass, Level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*AttributesSpecHandle.Data);
}
