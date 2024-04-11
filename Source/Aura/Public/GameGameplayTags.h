// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * GameGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FGameGameplayTags
{
public:
	static const FGameGameplayTags& Get() { return GameplayTags; }
	static void InitalizeNativeGameplayTags();

	FGameplayTag Attributes_Secondary_Armor;

protected:

private:
	static FGameGameplayTags GameplayTags;
};
