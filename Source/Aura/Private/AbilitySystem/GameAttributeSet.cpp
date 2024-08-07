// Copyright RaulSerranoDev


#include "AbilitySystem/GameAttributeSet.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "Player/GamePlayerController.h"
#include "AbilitySystem/GameAbilitySystemLibrary.h"
#include "Aura/GameLogChannels.h"
#include "Interaction/PlayerInterface.h"
#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

UGameAttributeSet::UGameAttributeSet()
{
	/* Primary Attributes */
	TagsToAttributes.Add(TAG_Attributes_Primary_Strength, GetStrengthAttribute());
	TagsToAttributes.Add(TAG_Attributes_Primary_Intelligence, GetIntelligenceAttribute());
	TagsToAttributes.Add(TAG_Attributes_Primary_Resilience, GetResilienceAttribute());
	TagsToAttributes.Add(TAG_Attributes_Primary_Vigor, GetVigorAttribute());

	/* Secondary Attributes */
	TagsToAttributes.Add(TAG_Attributes_Secondary_Armor, GetArmorAttribute());
	TagsToAttributes.Add(TAG_Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute());
	TagsToAttributes.Add(TAG_Attributes_Secondary_BlockChance, GetBlockChanceAttribute());
	TagsToAttributes.Add(TAG_Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute());
	TagsToAttributes.Add(TAG_Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute());
	TagsToAttributes.Add(TAG_Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute());
	TagsToAttributes.Add(TAG_Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute());
	TagsToAttributes.Add(TAG_Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute());
	TagsToAttributes.Add(TAG_Attributes_Secondary_MaxHealth, GetMaxHealthAttribute());
	TagsToAttributes.Add(TAG_Attributes_Secondary_MaxMana, GetMaxManaAttribute());

	/* Resistance Attributes */
	TagsToAttributes.Add(TAG_Attributes_Resistance_Fire, GetResistanceFireAttribute());
	TagsToAttributes.Add(TAG_Attributes_Resistance_Lightning, GetResistanceLightningAttribute());
	TagsToAttributes.Add(TAG_Attributes_Resistance_Arcane, GetResistanceArcaneAttribute());
	TagsToAttributes.Add(TAG_Attributes_Resistance_Physical, GetResistancePhysicalAttribute());

	/* Vital Attributes */
	TagsToAttributes.Add(TAG_Attributes_Vital_Health, GetHealthAttribute());
	TagsToAttributes.Add(TAG_Attributes_Vital_Mana, GetManaAttribute());
}

void UGameAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	// Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	// Resistance Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, ResistanceFire, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, ResistanceLightning, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, ResistanceArcane, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, ResistancePhysical, COND_None, REPNOTIFY_Always);

	// Vital Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGameAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}

void UGameAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter))return;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Props);
	}
}

void UGameAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bTopOfHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOfHealth = false;
	}
	if (Attribute == GetMaxManaAttribute() && bTopOfMana)
	{
		SetMana(GetMaxMana());
		bTopOfMana = false;
	}
}

void UGameAttributeSet::RefillVitalAttributes()
{
	bTopOfHealth = true;
	bTopOfMana = true;
}

void UGameAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source = causer of the effect, Target = target of the effect (owner of this AS)

	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (!Props.SourceController && Props.SourceAvatarActor)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = &Data.Target;
	}
}

void UGameAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0);

	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		const bool bFatal = NewHealth <= 0;
		if (bFatal)
		{
			if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
			{
				ASC->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_Debuff));
			}
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
			{
				const FVector DeathImpulse = UGameAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle);
				CombatInterface->Die(DeathImpulse);
			}
			SendXPEvent(Props);
		}
		else if (UGameAbilitySystemLibrary::ShouldHitReact(Props.EffectContextHandle))
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(TAG_Effects_HitReact);
			Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);

			const FVector KnockbackForce = UGameAbilitySystemLibrary::GetKnockbackForce(Props.EffectContextHandle);
			if (!KnockbackForce.IsNearlyZero(1.f))
			{
				Props.TargetCharacter->GetCharacterMovement()->StopMovementImmediately();
				Props.TargetCharacter->LaunchCharacter(KnockbackForce, true, true);
			}
		}

		const bool bBlocked = UGameAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		const bool bCritical = UGameAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);

		ShowFloatingText(Props, LocalIncomingDamage, bBlocked, bCritical);

		if (UGameAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
		{
			Debuff(Props);
		}
	}
}

void UGameAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlocked, bool bCritical) const
{
	if (Props.SourceCharacter == Props.TargetCharacter) return;

	if (AGamePlayerController* PC = Cast<AGamePlayerController>(Props.SourceController))
	{
		PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlocked, bCritical);
		return;
	}
	if (AGamePlayerController* PC = Cast<AGamePlayerController>(Props.TargetController))
	{
		PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlocked, bCritical);
	}
}

void UGameAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 XPReward = UGameAbilitySystemLibrary::GetXPReward(Props.TargetCharacter, ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter), ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter));
		FGameplayEventData Payload;
		Payload.EventTag = TAG_Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, TAG_Attributes_Meta_IncomingXP, Payload);
	}
}

void UGameAttributeSet::Debuff(const FEffectProperties& Props)
{
	const FGameGameplayTags GameplayTags = FGameGameplayTags::Get();

	const FGameplayTag DamageType = UGameAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
	const float DebuffDamage = UGameAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = UGameAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UGameAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);

	FGameplayEffectContextHandle EffectContext = Props.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(Props.SourceAvatarActor);

	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
	Effect->Period = DebuffFrequency;
	Effect->bExecutePeriodicEffectOnApplication = false;
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	const FGameplayTag DebuffTag = GameplayTags.DamageTypesToDebuffs[DamageType];
	FInheritedTagContainer TagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& Component = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	TagContainer.Added.AddTag(DebuffTag);
	if (DebuffTag.MatchesTagExact(TAG_Debuff_Stun))
	{
		TagContainer.Added.AddTag(TAG_Player_Block_CursorTrace);
		TagContainer.Added.AddTag(TAG_Player_Block_InputHeld);
		TagContainer.Added.AddTag(TAG_Player_Block_InputPressed);
		TagContainer.Added.AddTag(TAG_Player_Block_InputReleased);
	}
	Component.SetAndApplyTargetTagChanges(TagContainer);

	FGameplayEffectExecutionDefinition Execution;
	Execution.CalculationClass = UExecCalc_Damage::StaticClass();
	Effect->Executions.Add(Execution);

	FGameplayEffectSpec* EffectSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f);
	EffectSpec->SetSetByCallerMagnitude(DamageType, DebuffDamage);

	FGameGameplayEffectContext* AuraContext = static_cast<FGameGameplayEffectContext*>(EffectContext.Get());
	TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
	AuraContext->SetDamageType(DebuffDamageType);
	AuraContext->SetShouldHitReact(false);
	Props.TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpec);
}

void UGameAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0);

	// Source Character is the owner, since GA_ListenForEvents applies GE_EventBasedEffect, adding to IncosmingXP
	if (Props.SourceCharacter->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
	}
}

void UGameAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, Strength, OldStrength);
}

void UGameAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, Intelligence, OldIntelligence);
}

void UGameAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, Resilience, OldResilience);
}

void UGameAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, Vigor, OldVigor);
}

void UGameAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, Armor, OldArmor);
}

void UGameAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UGameAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, BlockChance, OldBlockChance);
}

void UGameAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UGameAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UGameAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UGameAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UGameAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UGameAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, MaxHealth, OldMaxHealth);
}

void UGameAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, MaxMana, OldMaxMana);
}

void UGameAttributeSet::OnRep_ResistanceFire(const FGameplayAttributeData& OldResistanceFire) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, ResistanceFire, OldResistanceFire);
}

void UGameAttributeSet::OnRep_ResistanceLightning(const FGameplayAttributeData& OldResistanceLightning) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, ResistanceLightning, OldResistanceLightning);
}

void UGameAttributeSet::OnRep_ResistanceArcane(const FGameplayAttributeData& OldResistanceArcane) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, ResistanceArcane, OldResistanceArcane);
}

void UGameAttributeSet::OnRep_ResistancePhysical(const FGameplayAttributeData& OldResistancePhysical) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, ResistancePhysical, OldResistancePhysical);
}

void UGameAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, Health, OldHealth);
}

void UGameAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGameAttributeSet, Mana, OldMana);
}
