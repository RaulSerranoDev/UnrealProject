// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GameGameplayAbility.h"
#include "PassiveAbility.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UPassiveAbility : public UGameGameplayAbility
{
	GENERATED_BODY()

public:
	UPassiveAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	void ReceiveDeactivate(const FGameplayTag& AbilityTag);

};
