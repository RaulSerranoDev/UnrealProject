// Copyright RaulSerranoDev


#include "AbilitySystem/GameAbilitySystemLibrary.h"
#include "UI/HUD/GameHUD.h"
#include "Player/GamePlayerState.h"
#include "UI/WidgetController/GameWidgetController.h"

UOverlayWidgetController* UGameAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		if (AGameHUD* GameHUD = Cast<AGameHUD>(PC->GetHUD()))
		{
			AGamePlayerState* PS = PC->GetPlayerState<AGamePlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return GameHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}
