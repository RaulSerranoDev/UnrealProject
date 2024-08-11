// Copyright RaulSerranoDev


#include "AbilitySystem/Abilities/FireBlast.h"

#include "AbilitySystem/GameAbilitySystemLibrary.h"
#include "Actor/FireBallProjectile.h"

TArray<AFireBallProjectile*> UFireBlast::SpawnFireBalls()
{
	TArray<AFireBallProjectile*> FireBalls;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UGameAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFireBalls);

	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());

		AFireBallProjectile* FireBall = GetWorld()->SpawnActorDeferred<AFireBallProjectile>(
			FireBallProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->ReturnToActor = GetAvatarActorFromActorInfo();

		FireBalls.Add(FireBall);

		FireBall->FinishSpawning(SpawnTransform);
	}

	return FireBalls;
}

int32 UFireBlast::GetNumFireBalls(int32 Level) const
{
	return NumFireBalls;
}
