// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameplayTagContainer.h"
#include "PassiveNiagaraComponent.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UPassiveNiagaraComponent();

protected:
	virtual void BeginPlay() override;

	void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate);

public:
	UPROPERTY(VisibleAnywhere)
	FGameplayTag PassiveSpellTag;

};
