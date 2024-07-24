// Copyright RaulSerranoDev


#include "AbilitySystem/Abilities/ProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

#include "Actor/Projectile.h"
#include "Interaction/CombatInterface.h"
#include "GameGameplayTags.h"

FString UProjectileSpell::GetDescription(int32 Level)
{
	int32 MinDamage, MaxDamage;
	GetDamageRange(TAG_Damage_Fire, Level, MinDamage, MaxDamage);

	return FString::Printf(TEXT("<Title>FIRE BOLT</>\n\nLaunches %d bolts of fire, exploding on impact and dealing: <Damage>%d - %d</> fire damage with a chance to burn\n\n<Small>Level: </><Level>%d</>"),
		FMath::Min(Level, MaxNumProjectiles), MinDamage, MaxDamage, Level);
}

FString UProjectileSpell::GetNextLevelDescription(int32 Level)
{
	int32 MinDamage, MaxDamage;
	GetDamageRange(TAG_Damage_Fire, Level, MinDamage, MaxDamage);

	return FString::Printf(TEXT("<Title>NEXT LEVEL:</>\n\nLaunches %d bolts of fire, exploding on impact and dealing: <Damage>%d - %d</> fire damage with a chance to burn\n\n<Small>Level: </><Level>%d</>"),
		FMath::Min(Level, MaxNumProjectiles), MinDamage, MaxDamage, Level);
}

void UProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, const FVector Offset, bool bOverridePitch, float PitchOverride)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	APawn* Pawn = Cast<APawn>(GetAvatarActorFromActorInfo());

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag)
		+ Offset;
	FRotator Rotation = (ProjectileTargetLocation - Pawn->GetActorLocation()).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Pawn,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	const UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();

	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Projectile);
	EffectContextHandle.AddActors(Actors);
	FHitResult HitResult;
	HitResult.Location = ProjectileTargetLocation;
	EffectContextHandle.AddHitResult(HitResult);
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

	for (auto& Pair : DamageTypes)
	{
		const float ScaledMagnitudeMin = Pair.Value.DamageMin.GetValueAtLevel(GetAbilityLevel());
		const float ScaledMagnitudeMax = Pair.Value.DamageMax.GetValueAtLevel(GetAbilityLevel());
		const float ScaledDamage = FMath::RandRange(ScaledMagnitudeMin, ScaledMagnitudeMax);

		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
	}

	Projectile->DamageEffectSpecHandle = SpecHandle;

	Projectile->FinishSpawning(SpawnTransform);
}
