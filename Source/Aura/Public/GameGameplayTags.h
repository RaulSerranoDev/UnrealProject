// Copyright RaulSerranoDev

#pragma once

#include "NativeGameplayTags.h"

/** Declare the gameplay tags. */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Primary_Strength)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Primary_Intelligence)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Primary_Resilience)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Primary_Vigor)

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Secondary_Armor)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Secondary_ArmorPenetration)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Secondary_BlockChance)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Secondary_CriticalHitChance)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Secondary_CriticalHitDamage)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Secondary_CriticalHitResistance)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Secondary_HealthRegeneration)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Secondary_ManaRegeneration)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Secondary_MaxHealth)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Secondary_MaxMana)

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Vital_Health)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Vital_Mana)

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_LMB);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_RMB);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_1);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_2);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_3);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_4);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Damage);

//#include "CoreMinimal.h"
//#include "GameplayTagContainer.h"

/**
 * GameGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

 //struct FGameGameplayTags
 //{
 //public:
 //	static const FGameGameplayTags& Get() { return GameplayTags; }
 //	static void InitalizeNativeGameplayTags();
 //
 //	FGameplayTag Attributes_Secondary_Armor;
 //
 //	FGameplayTag Attributes_Vital_Health;
 //	FGameplayTag Attributes_Vital_Mana;
 //
 //protected:
 //
 //private:
 //	static FGameGameplayTags GameplayTags;
 //};
