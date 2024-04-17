// Copyright RaulSerranoDev


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "Player/GamePlayerController.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask< UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	Super::Activate();

	AGamePlayerController* PC = Cast<AGamePlayerController>(Ability->GetCurrentActorInfo()->PlayerController);
	ValidData.Broadcast(PC->GetCursorHit().Location);
}
