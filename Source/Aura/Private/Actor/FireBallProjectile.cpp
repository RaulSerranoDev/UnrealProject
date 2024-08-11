// Copyright RaulSerranoDev


#include "Actor/FireBallProjectile.h"

#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayCueManager.h"

#include "AbilitySystem/GameAbilitySystemLibrary.h"
#include "GameGameplayTags.h"

AFireBallProjectile::AFireBallProjectile()
{
	OutgoingTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("OutgoingTimeline"));
	UpdateOutgoing.BindUFunction(this, FName("OutgoingUpdate"));
	FinishedOutgoing.BindUFunction(this, FName("OutgoingFinished"));

	ReturningTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ReturningTimeline"));
	UpdateReturn.BindUFunction(this, FName("ReturnUpdate"));
}

void AFireBallProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetupTimer();
		OutgoingTimeline->Play();
	}
}

void AFireBallProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor) || IgnoreList.Contains(OtherActor)) return;

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			DamageEffectParams.TargetASC = TargetASC;
			UGameAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}

		IgnoreList.Add(OtherActor);
	}
}

void AFireBallProjectile::OnHit()
{
	if (GetOwner())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), TAG_GameplayCue_FireBlast, CueParams);
	}
	bClientHit = true;
}

void AFireBallProjectile::SetupTimer()
{
	if (OutgoingCurve && ReturningCurve)
	{
		OutgoingTimeline->AddInterpFloat(OutgoingCurve, UpdateOutgoing, FName("Alpha"));
		OutgoingTimeline->SetTimelineFinishedFunc(FinishedOutgoing);
		OutgoingTimeline->SetLooping(false);
		OutgoingTimeline->SetIgnoreTimeDilation(true);

		ReturningTimeline->AddInterpFloat(ReturningCurve, UpdateReturn, FName("Alpha"));
		ReturningTimeline->SetLooping(false);
		ReturningTimeline->SetIgnoreTimeDilation(true);
	}

	StartLocation = GetActorLocation();
	EndLocation = StartLocation + (GetActorForwardVector() * Distance);
}

void AFireBallProjectile::OutgoingUpdate(float Value)
{
	const FVector LerpLocation = UKismetMathLibrary::VLerp(StartLocation, EndLocation, Value);
	SetActorLocation(LerpLocation);
}

void AFireBallProjectile::OutgoingFinished()
{
	ReturningTimeline->Play();
	IgnoreList.Empty();
}

void AFireBallProjectile::ReturnUpdate(float Value)
{
	if (IsValid(ReturnToActor))
	{
		const FVector LerpLocation = UKismetMathLibrary::VLerp(EndLocation, ReturnToActor->GetActorLocation(), Value);
		SetActorLocation(LerpLocation);
		const float DistanceToReturn = GetDistanceTo(ReturnToActor);
		if (DistanceToReturn < ExplodingDistance)
		{
			OnHit();
			OnHitEvent();
			Destroy();
		}
	}
	else
	{
		OnHit();
		OnHitEvent();
		Destroy();
	}
}
