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

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Resistance_Fire)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Resistance_Lightning)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Resistance_Arcane)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Resistance_Physical)

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Meta_IncomingXP)

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Vital_Health)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attributes_Vital_Mana)

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_LMB);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_RMB);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_1);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_2);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_3);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_4);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_Passive_1);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_Passive_2);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Damage_Fire);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Damage_Lightning);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Damage_Arcane);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Damage_Physical);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Abilities);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Abilities_Attack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Abilities_Summon);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Abilities_HitReact);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Abilities_Status);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Abilities_Status_Locked);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Abilities_Status_Eligible);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Abilities_Status_Unlocked);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Abilities_Status_Equipped);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Abilities_Type_Offensive);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Abilities_Type_Passive);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Abilities_Type_None);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Abilities_Fire_FireBolt);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Abilities_Lightning_Electrocute);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Fire_FireBolt);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_CombatSocket_Weapon);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_CombatSocket_LeftHand);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_CombatSocket_RightHand);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_CombatSocket_Tail);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Montage_Attack_1);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Montage_Attack_2);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Montage_Attack_3);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Montage_Attack_4);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effects_HitReact);


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

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

private:
	static FGameGameplayTags GameplayTags;

};
