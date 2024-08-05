// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DamageGameplayAbility.h"
#include "ProjectileSpell.generated.h"

class AProjectile;
class UGameplayEffect;
struct FGameplayTag;

/**
 *
 */
UCLASS()
class AURA_API UProjectileSpell : public UDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, const FVector Offset = FVector::ZeroVector, bool bOverridePitch = false, float PitchOverride = 0.f);

	UFUNCTION(BlueprintPure, meta = (HidePin = "Target"))
	int32 GetNumProjectiles(int32 Level = 1) const;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, const FVector Offset = FVector::ZeroVector, bool bOverridePitch = false, float PitchOverride = 0.f, AActor* HomingTarget = nullptr, bool bShowDebug = false);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	FScalableFloat NumProjectiles = 1;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float ProjectileSpread = 90.f;

};
