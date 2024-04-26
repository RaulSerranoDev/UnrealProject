// Copyright RaulSerranoDev


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"

#include "AbilitySystem/GameAttributeSet.h"
#include "GameGameplayTags.h"

struct GameDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);

	//FGameplayEffectAttributeCaptureDefinition SourceIntDef;
	//FGameplayEffectAttributeCaptureDefinition TargetIntDef;

	GameDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGameAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGameAttributeSet, BlockChance, Target, false);

		//SourceIntDef = FGameplayEffectAttributeCaptureDefinition(UGameAttributeSet::GetIntelligenceAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
		//TargetIntDef = FGameplayEffectAttributeCaptureDefinition(UGameAttributeSet::GetIntelligenceAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	}
};

static const GameDamageStatics& DamageStatics()
{
	static GameDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = SourceTags;
	EvalParams.TargetTags = TargetTags;

	// Get Damage Set By Caller Magnitude
	float Damage = Spec.GetSetByCallerMagnitude(TAG_Damage);

	// Capture BlockChance on Target, and determine if there was a successful Block
	// If Block, halve the damage.
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvalParams, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);
	const bool bBlocked = bBlockErrorTolerance ? FMath::RandRange(1, 100) <= TargetBlockChance : FMath::FRandRange(UE_SMALL_NUMBER, 100.0f) <= TargetBlockChance;
	Damage = bBlocked ? Damage * 0.5f : Damage;

	const FGameplayModifierEvaluatedData EvalData(UGameAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvalData);
}
