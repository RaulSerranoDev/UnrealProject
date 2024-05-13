// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include"GameplayTagContainer.h"
#include "CombatInterface.generated.h"

class UAnimMontage;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* CriticalImpactSound = nullptr;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FGameplayTag GetSpellMontageTag();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetBloodEffect(bool bIsCritical);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);

	virtual void Die() = 0;
	virtual int32 GetPlayerLevel() const;
};
