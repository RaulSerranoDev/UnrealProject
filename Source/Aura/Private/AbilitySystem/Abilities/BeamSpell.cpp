// Copyright RaulSerranoDev


#include "AbilitySystem/Abilities/BeamSpell.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Interaction/CombatInterface.h"
#include "AbilitySystem/GameAbilitySystemLibrary.h"

void UBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UBeamSpell::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UBeamSpell::TraceFirstTarget()
{
	check(OwnerCharacter);
	if (!OwnerCharacter->Implements<UCombatInterface>()) return;

	if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
	{
		const FVector SocketLocation = Weapon->GetSocketLocation("WeaponSocket");
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(OwnerCharacter);
		FHitResult HitResult;
		UKismetSystemLibrary::SphereTraceSingle(
			OwnerCharacter,
			SocketLocation,
			MouseHitLocation,
			10.f,
			TraceTypeQuery_MAX,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResult,
			true);

		if (HitResult.bBlockingHit)
		{
			MouseHitLocation = HitResult.ImpactPoint;
			MouseHitActor = HitResult.GetActor();
		}
	}
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor))
	{
		CombatInterface->GetOnDeathDelegate().AddUniqueDynamic(this, &ThisClass::PrimaryTargetDied);
	}
}

void UBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> OverlappingActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(MouseHitActor);
	UGameAbilitySystemLibrary::GetLivePlayersWithinRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsToIgnore,
		850.f,
		MouseHitActor->GetActorLocation());

	int32 NumAdditionalTargets = NumShockTargets.GetValueAtLevel(GetAbilityLevel()) - 1;

	UGameAbilitySystemLibrary::GetClosestTargets(MouseHitActor->GetActorLocation(), NumAdditionalTargets, OverlappingActors, OutAdditionalTargets);

	for (AActor* Target : OutAdditionalTargets)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
		{
			CombatInterface->GetOnDeathDelegate().AddUniqueDynamic(this, &ThisClass::AdditionalTargetDied);
		}
	}
}

void UBeamSpell::RemoveOnDeathNotify(AActor* Actor)
{
	if (const auto CombatInterface = Cast<ICombatInterface>(Actor))
	{
		CombatInterface->GetOnDeathDelegate().RemoveAll(this);
	}
}

int32 UBeamSpell::GetNumShockAdditionalTargets(int32 Level) const
{
	return NumShockTargets.GetValueAtLevel(Level) - 1;
}
