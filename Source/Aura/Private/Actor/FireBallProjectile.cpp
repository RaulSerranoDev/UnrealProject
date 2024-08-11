// Copyright RaulSerranoDev


#include "Actor/FireBallProjectile.h"

#include "Kismet/KismetMathLibrary.h"

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

void AFireBallProjectile::OutgoingFinished() const
{
	ReturningTimeline->Play();
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
			Destroy();
		}
	}
	else
	{
		OnHit();
		Destroy();
	}
}
