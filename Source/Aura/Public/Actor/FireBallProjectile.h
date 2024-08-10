// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "Actor/Projectile.h"
#include "FireBallProjectile.generated.h"

/**
 *
 */
UCLASS()
class AURA_API AFireBallProjectile : public AProjectile
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
