// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/HighlightInterface.h"
#include "AttributeDelegateTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

class UWidgetComponent;
struct FGameplayTag;
class UBehaviorTree;
class AGameAIController;

/**
 *
 */
UCLASS()
class AURA_API AEnemyCharacter : public ACharacterBase, public IHighlightInterface, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	virtual void PossessedBy(AController* NewController) override;

	/** Highlight Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** end Highlight Interface */

	/** Combat Interface */
	virtual int32 GetPlayerLevel() const override;
	virtual void Die() override;
	/** end Combat Interface */

	/** Enemy Interface */
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	AActor* GetCombatTarget_Implementation() const override;
	/** end Enemy Interface */

	virtual void MulticastHandleDeath_Implementation() override;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

private:
	void InitHitReact();
	void InitHealthBarWidget();

public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AGameAIController> GameAIController;

};
