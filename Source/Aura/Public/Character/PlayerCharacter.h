// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class AURA_API APlayerCharacter : public ACharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	/**
	 * Init ability actor info for the server
	 */
	virtual void PossessedBy(AController* NewController) override;

	/**
	 * Init ability actor info for the client
	 */
	virtual void OnRep_PlayerState() override;

	/** Player Interface */
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual void LevelUp_Implementation() override;
	/** end Player Interface */

	/** Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() const override;
	/** end Combat Interface */

protected:
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Setup|Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Setup|Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Setup|Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

};
