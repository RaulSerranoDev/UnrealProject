// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DamageGameplayAbility.h"
#include "FireBlast.generated.h"

class AFireBallProjectile;

/**
 *
 */
UCLASS()
class AURA_API UFireBlast : public UDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TArray<AFireBallProjectile*> SpawnFireBalls();

protected:
	UFUNCTION(BlueprintPure, meta = (HidePin = "Target"))
	int32 GetNumFireBalls(int32 Level = 1) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	int32 NumFireBalls = 12;

private:
	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	TSubclassOf<AFireBallProjectile> FireBallProjectileClass;
};
