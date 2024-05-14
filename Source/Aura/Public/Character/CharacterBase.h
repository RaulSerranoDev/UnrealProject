// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "CharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UNiagaraSystem;

UCLASS(Abstract)
class AURA_API ACharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/** Combat Interface */
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation(bool bIsCritical) override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag);

	virtual void Die() override;
	/** Combat Interface */

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;

	void AddCharacterAbilities();

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

private:
	bool GetSocketLocationOnMesh(const FGameplayTag& MontageTag, const TMap<FGameplayTag, FName>& SocketMeshMap, const USkeletalMeshComponent* SocketMesh, FVector& Location) const;

public:
	UPROPERTY(EditAnywhere, Category = "Setup|Combat")
	TArray<FTaggedMontage> AttackMontages;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Setup|Combat")
	TMap<FGameplayTag, FName> WeaponTagNameSocketMap;

	UPROPERTY(EditAnywhere, Category = "Setup|Combat")
	TMap<FGameplayTag, FName> MeshTagNameSocketMap;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAttributeSet> AttributeSet;

	/* Dissolve Effects */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Combat")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Combat")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	/* !Dissolve Effects */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Combat")
	TObjectPtr<UNiagaraSystem> BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Combat")
	TObjectPtr<UNiagaraSystem> CriticalBloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Combat")
	TObjectPtr<USoundBase> DeathSound;

	bool bDead = false;

private:
	UPROPERTY(EditAnywhere, Category = "Setup|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Setup|Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

};
