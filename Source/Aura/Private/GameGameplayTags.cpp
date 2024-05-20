// Copyright RaulSerranoDev


#include "GameGameplayTags.h"

/** Define and expose the gameplay tags to other modules and code. */

/*
*  Primary Attributes
*/

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Primary_Strength,
	"Attributes.Primary.Strength",
	"Increases physical damage"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Primary_Intelligence,
	"Attributes.Primary.Intelligence",
	"Increases magical damage"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Primary_Resilience,
	"Attributes.Primary.Resilience",
	"Increases Armor and Armor Penetration"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Primary_Vigor,
	"Attributes.Primary.Vigor",
	"Increases Health"
)

/*
*  Secondary Attributes
*/

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_Armor,
	"Attributes.Secondary.Armor",
	"Reduces damage taken, improves Block Chance"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_ArmorPenetration,
	"Attributes.Secondary.ArmorPenetration",
	"Ignored Percentage of enemy Armor, increases Critical Hit Chance"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_BlockChance,
	"Attributes.Secondary.BlockChance",
	"Chance to cut incoming damage in half"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_CriticalHitChance,
	"Attributes.Secondary.CriticalHitChance",
	"Chance to double damage plus critical hit bonus"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_CriticalHitDamage,
	"Attributes.Secondary.CriticalHitDamage",
	"Bonus damage added when a critical hit is scored"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_CriticalHitResistance,
	"Attributes.Secondary.CriticalHitResistance",
	"Reduces Critical Hit Chance of attacking enemies"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_HealthRegeneration,
	"Attributes.Secondary.HealthRegeneration",
	"Amount of Health regenerated every 1 second"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_ManaRegeneration,
	"Attributes.Secondary.ManaRegeneration",
	"Amount of Mana regenerated every 1 second"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_MaxHealth,
	"Attributes.Secondary.MaxHealth",
	"Maximum amount of Health obtainable"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_MaxMana,
	"Attributes.Secondary.MaxMana",
	"Maximum amount of Mana obtainable"
)

/*
*  Resistance Attributes
*/

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Resistance_Fire,
	"Attributes.Resistance.Fire",
	"Fire Resistance"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Resistance_Lightning,
	"Attributes.Resistance.Lightning",
	"Lightning Resistance"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Resistance_Arcane,
	"Attributes.Resistance.Arcane",
	"Arcane Resistance"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Resistance_Physical,
	"Attributes.Resistance.Physical",
	"Physical Resistance"
)

/*
*  Vital Attributes
*/

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Vital_Health,
	"Attributes.Vital.Health",
	"Amount of damage a player can take before death"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Vital_Mana,
	"Attributes.Vital.Mana",
	"A resource used to cast spells"
)

/*
*  Input Tags
*/

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_InputTag,
	"InputTag",
	"Input Tag"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_InputTag_LMB,
	"InputTag.LMB",
	"Input Tag for Left Mouse Button"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_InputTag_RMB,
	"InputTag.RMB",
	"Input Tag for Right Mouse Button"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_InputTag_1,
	"InputTag.1",
	"Input Tag for 1 key"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_InputTag_2,
	"InputTag.2",
	"Input Tag for 2 key"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_InputTag_3,
	"InputTag.3",
	"Input Tag for 3 key"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_InputTag_4,
	"InputTag.4",
	"Input Tag for 4 key"
)

/*
*  Damage Types
*/

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Damage,
	"Damage",
	"Damage"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Damage_Fire,
	"Damage.Fire",
	"Fire Damage Type"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Damage_Lightning,
	"Damage.Lightning",
	"Lightning Damage Type"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Damage_Arcane,
	"Damage.Arcane",
	"Arcane Damage Type"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Damage_Physical,
	"Damage.Physical",
	"Physical Damage Type"
)

/*
*  Abilities
*/

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities,
	"Abilities",
	"Abilities Tag"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Attack,
	"Abilities.Attack",
	"Attack Ability Tag"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Summon,
	"Abilities.Summon",
	"Summon Ability Tag"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Fire_FireBolt,
	"Abilities.Fire.FireBolt",
	"FireBolt Ability Tag"
)

/*
*  Combat Sockets
*/

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_CombatSocket_Weapon,
	"CombatSocket.Weapon",
	"Weapon"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_CombatSocket_LeftHand,
	"CombatSocket.LeftHand",
	"Left Hand"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_CombatSocket_RightHand,
	"CombatSocket.RightHand",
	"Right Hand"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_CombatSocket_Tail,
	"CombatSocket.Tail",
	"Tail"
)

/*
*  Montage Tags
*/

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Montage_Attack_1,
	"Montage.Attack.1",
	"Attack 1"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Montage_Attack_2,
	"Montage.Attack.2",
	"Attack 2"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Montage_Attack_3,
	"Montage.Attack.3",
	"Attack 3"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Montage_Attack_4,
	"Montage.Attack.4",
	"Attack 4"
)

/*
*  Effects
*/

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Effects_HitReact,
	"Effects.HitReact",
	"Tag granted when Hit Reacting"
)


FGameGameplayTags FGameGameplayTags::GameplayTags;

void FGameGameplayTags::InitalizeNativeGameplayTags()
{
	GameplayTags.DamageTypesToResistances.Add(TAG_Damage_Fire, TAG_Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(TAG_Damage_Lightning, TAG_Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(TAG_Damage_Arcane, TAG_Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(TAG_Damage_Physical, TAG_Attributes_Resistance_Physical);
}
