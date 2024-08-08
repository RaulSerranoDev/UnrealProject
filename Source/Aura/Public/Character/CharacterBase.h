// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "CharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UNiagaraSystem;
class UDebuffNiagaraComponent;

UCLASS(Abstract)
class AURA_API ACharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/** Combat Interface */
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation(bool bIsCritical) override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void AddMinionCount_Implementation(int32 Amount) override;
	virtual ECharacterClass GetCharacterClass_Implementation() const override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() const override;

	virtual void Die(const FVector& DeathImpulse) override;

	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	virtual FOnDeath& GetOnDeathDelegate() override;

	/** Combat Interface */

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

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

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UFUNCTION()
	virtual void OnRep_Stunned();

	UFUNCTION()
	virtual void OnRep_Burned();

private:
	bool GetSocketLocationOnMesh(const FGameplayTag& MontageTag, const TMap<FGameplayTag, FName>& SocketMeshMap, const USkeletalMeshComponent* SocketMesh, FVector& Location) const;

public:
	UPROPERTY(EditAnywhere, Category = "Setup|Combat")
	TArray<FTaggedMontage> AttackMontages;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Setup|Combat", meta = (Categories = "CombatSocket"))
	TMap<FGameplayTag, FName> WeaponTagNameSocketMap;

	UPROPERTY(EditAnywhere, Category = "Setup|Combat", meta = (Categories = "CombatSocket"))
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Combat")
	float BaseWalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(ReplicatedUsing = OnRep_Stunned, BlueprintReadOnly)
	bool bIsStunned = false;

	UPROPERTY(ReplicatedUsing = OnRep_Burned, BlueprintReadOnly)
	bool bIsBurned = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent;

private:
	UPROPERTY(EditAnywhere, Category = "Setup|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Setup|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Setup|Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

protected:
	FOnASCRegistered OnASCRegistered;
	FOnDeath OnDeath;

private:
	int32 MinionCount = 0;

	bool bDead = false;

};
