// Copyright RaulSerranoDev


#include "AbilitySystem/Abilities/ProjectileSpell.h"

#include "Actor/Projectile.h"
#include "Interaction/CombatInterface.h"

void UProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!CombatInterface) return;

	const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);

	// TODO: Set the Projectile Rotation

	AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	// TODO: Give the Projectile a Gameplay Effect Spec for causing Damage

	Projectile->FinishSpawning(SpawnTransform);
}
