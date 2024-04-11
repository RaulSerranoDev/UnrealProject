// Copyright RaulSerranoDev


#include "GameGameplayTags.h"
#include "GameplayTagsManager.h"

FGameGameplayTags FGameGameplayTags::GameplayTags;

void FGameGameplayTags::InitalizeNativeGameplayTags()
{
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage taken, improves Bloch Chance"));
}
