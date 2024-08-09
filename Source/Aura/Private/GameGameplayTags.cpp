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
*  Meta Attributes
*/

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Meta_IncomingXP,
	"Attributes.Meta.IncomingXP",
	"Incoming XP Meta Attribute"
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

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_InputTag_Passive_1,
	"InputTag.Passive.1",
	"Input Tag for Passive Ability 1"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_InputTag_Passive_2,
	"InputTag.Passive.2",
	"Input Tag for Passive Ability 2"
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

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_None,
	"Abilities.None",
	"None Ability Tag"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Attack,
	"Abilities.Attack",
	"Attack Ability Tag"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Summon,
	"Abilities.Summon",
	"Summon Ability Tag"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_HitReact,
	"Abilities.HitReact",
	"HitReact Ability Tag"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Status,
	"Abilities.Status",
	"Status"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Status_Locked,
	"Abilities.Status.Locked",
	"Locked Status"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Status_Eligible,
	"Abilities.Status.Eligible",
	"Elegible Status"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Status_Unlocked,
	"Abilities.Status.Unlocked",
	"Unlocked Status"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Status_Equipped,
	"Abilities.Status.Equipped",
	"Equipped Status"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Type_Offensive,
	"Abilities.Type.Offensive",
	"Offensive Type"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Type_Passive,
	"Abilities.Type.Passive",
	"Passive Type"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Type_None,
	"Abilities.Type.None",
	"None Type"
)

/*
* Offensive Spells
*/

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Fire_FireBolt,
	"Abilities.Fire.FireBolt",
	"FireBolt Ability Tag"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Lightning_Electrocute,
	"Abilities.Lightning.Electrocute",
	"Electrocute Ability Tag"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Arcane_ArcaneShards,
	"Abilities.Arcane.ArcaneShards",
	"ArcaneShards Ability Tag"
)

/*
*  Passive Spells
*/

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Passive_HaloOfProtection,
	"Abilities.Passive.HaloOfProtection",
	"HaloOfProtection"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Passive_LifeSiphon,
	"Abilities.Passive.LifeSiphon",
	"LifeSiphon"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Passive_ManaSiphon,
	"Abilities.Passive.ManaSiphon",
	"ManaSiphon"
)

/*
*  Debuffs
*/

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Debuff,
	"Debuff",
	"Debuff Tag"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Debuff_Burn,
	"Debuff.Burn",
	"Debuff for Fire Damage"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Debuff_Stun,
	"Debuff.Stun",
	"Debuff for Lightning Damage"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Debuff_Arcane,
	"Debuff.Arcane",
	"Debuff for Arcane Damage"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Debuff_Physical,
	"Debuff.Physical",
	"Debuff for Physical Damage"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Debuff_Chance,
	"Debuff.Chance",
	"Debuff Chance"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Debuff_Damage,
	"Debuff.Damage",
	"Debuff Damage"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Debuff_Duration,
	"Debuff.Duration",
	"Debuff Duration"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Debuff_Frequency,
	"Debuff.Frequency",
	"Debuff Frequency"
)

/*
*  Cooldown
*/

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Fire_FireBolt,
	"Cooldown.Fire.FireBolt",
	"FireBolt Cooldown Tag"
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

/*
*  Player Tags
*/

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Player_Block_InputPressed,
	"Player.Block.InputPressed",
	"Block Input Pressed callback under the cursor"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Player_Block_InputHeld,
	"Player.Block.InputHeld",
	"Block Input Held callback the cursor"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Player_Block_InputReleased,
	"Player.Block.InputReleased",
	"Block Input Released callback under the cursor"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Player_Block_InputReleasedMovement,
	"Player.Block.InputReleasedMovement",
	"Block Input Release Movement under the cursor"
)

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Player_Block_CursorTrace,
	"Player.Block.CursorTrace",
	"Block tracing under the cursor"
)

FGameGameplayTags FGameGameplayTags::GameplayTags;

void FGameGameplayTags::InitalizeNativeGameplayTags()
{
	/*
	*  Map of Damage Types to Resistances
	*/
	GameplayTags.DamageTypesToResistances.Add(TAG_Damage_Fire, TAG_Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(TAG_Damage_Lightning, TAG_Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(TAG_Damage_Arcane, TAG_Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(TAG_Damage_Physical, TAG_Attributes_Resistance_Physical);

	/*
	*  Map of Damage Types to Debuffs
	*/
	GameplayTags.DamageTypesToDebuffs.Add(TAG_Damage_Fire, TAG_Debuff_Burn);
	GameplayTags.DamageTypesToDebuffs.Add(TAG_Damage_Lightning, TAG_Debuff_Stun);
	GameplayTags.DamageTypesToDebuffs.Add(TAG_Damage_Arcane, TAG_Debuff_Arcane);
	GameplayTags.DamageTypesToDebuffs.Add(TAG_Damage_Physical, TAG_Debuff_Physical);
}
