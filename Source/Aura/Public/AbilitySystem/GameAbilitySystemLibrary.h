// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "GameAbilitySystemLibrary.generated.h"

struct FWidgetControllerParams;
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class USpellMenuWidgetController;
class UAbilitySystemComponent;
struct FGameplayEffectContextHandle;

/**
 *
 */
UCLASS()
class AURA_API UGameAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "GameAbilitySystemLibrary|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AGameHUD*& OutHUD);

	UFUNCTION(BlueprintPure, Category = "GameAbilitySystemLibrary|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "GameAbilitySystemLibrary|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "GameAbilitySystemLibrary|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "GameAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "GameAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category = "GameAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "GameAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "GameAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "GameAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bBlocked);

	UFUNCTION(BlueprintCallable, Category = "GameAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bCritical);

	// Function to return the magnitude tags for SetByCaller Magnitude for gameplay Effects
	UFUNCTION(BlueprintPure, Category = "GameAbilitySystemLibrary|GameplayEffects")
	static TArray<FGameplayTag> CallerMagnitudeTags(TSubclassOf<UGameplayEffect> GameplayEffect);

	UFUNCTION(BlueprintCallable, Category = "GameAbilitySystemLibrary|GameplayMechanics", meta = (WorldContext = "WorldContextObject"))
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin, FName FilterTag = NAME_None, bool bShowDebugSphere = false);

	UFUNCTION(BlueprintPure, Category = "GameAbilitySystemLibrary|GameplayMechanics")
	static bool IsOnSameTeam(const AActor* FirstActor, const AActor* SecondActor);

	static int32 GetXPReward(const UObject* WorldContextObject, ECharacterClass CharacterClass, const int32& Level);

private:
	static void ApplyGameplayEffectHelper(TSubclassOf<UGameplayEffect> GEClass, int Level, FGameplayEffectContextHandle ContextHandle, UAbilitySystemComponent* ASC);

};
