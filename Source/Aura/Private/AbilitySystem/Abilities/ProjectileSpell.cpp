// Copyright RaulSerranoDev


#include "AbilitySystem/Abilities/ProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Actor/Projectile.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/GameAbilitySystemLibrary.h"

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

	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

	Projectile->FinishSpawning(SpawnTransform);
}

int32 UProjectileSpell::GetNumProjectiles(int32 Level) const
{
	return NumProjectiles.GetValueAtLevel(Level);
}

void UProjectileSpell::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, const FVector Offset,
	bool bOverridePitch, float PitchOverride, AActor* HomingTarget, bool bShowDebug)
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

	const FVector Forward = Rotation.Vector();
	if (bShowDebug)
	{
		const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread / 2.f, FVector::UpVector);
		const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread / 2.f, FVector::UpVector);
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Forward * 100.f, 5, FLinearColor::White, 120, 1);
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + LeftOfSpread * 100.f, 5, FLinearColor::Gray, 120, 1);
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + RightOfSpread * 100.f, 5, FLinearColor::Gray, 120, 1);
	}

	TArray<FRotator> Rotations = UGameAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, GetNumProjectiles(GetAbilityLevel()));
	for (FRotator& Rotator : Rotations)
	{
		if (bShowDebug)
		{
			FVector Start = SocketLocation + FVector(0, 0, 10);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, Start + Rotator.Vector() * 75.f, 5, FLinearColor::Blue, 120, 1);
		}

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotator.Quaternion());

		AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Pawn,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		Projectile->FinishSpawning(SpawnTransform);
	}
}
