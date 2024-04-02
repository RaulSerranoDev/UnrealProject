// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class AURA_API APlayerCharacter : public ACharacterBase
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

protected:
	virtual void InitAbilityActorInfo() override;

};
