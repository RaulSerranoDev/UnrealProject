// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/HighlightInterface.h"
#include "EnemyCharacter.generated.h"

/**
 *
 */
UCLASS()
class AURA_API AEnemyCharacter : public ACharacterBase, public IHighlightInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	/** Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** end Enemy Interface */

	/** Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/** end Combat Interface */

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

};
