// Copyright RaulSerranoDev


#include "Actor/FireBallProjectile.h"

void AFireBallProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AFireBallProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
