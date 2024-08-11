// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "Actor/Projectile.h"
#include "Components/TimelineComponent.h"
#include "FireBallProjectile.generated.h"

class UTimelineComponent;

/**
 *
 */
UCLASS()
class AURA_API AFireBallProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	AFireBallProjectile();

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnHitEvent();

private:
	void SetupTimer();

	UFUNCTION()
	void OutgoingUpdate(float Value);

	UFUNCTION()
	void OutgoingFinished();

	UFUNCTION()
	void ReturnUpdate(float Value);

private:
	FOnTimelineFloat UpdateOutgoing{};
	FOnTimelineEvent FinishedOutgoing{};
	FOnTimelineFloat UpdateReturn{};

public:
	TObjectPtr<AActor> ReturnToActor;

	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams ExplosionDamageParams;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Timelines")
	float Distance = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Timelines")
	float ExplodingDistance = 250.f;

	UPROPERTY(EditDefaultsOnly, Category = "Timelines")
	TObjectPtr<UCurveFloat> OutgoingCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Timelines")
	TObjectPtr<UCurveFloat> ReturningCurve;

	UPROPERTY()
	TObjectPtr<UTimelineComponent> OutgoingTimeline;

	UPROPERTY()
	TObjectPtr<UTimelineComponent> ReturningTimeline;

	FVector StartLocation = FVector::ZeroVector;
	FVector EndLocation = FVector::ZeroVector;

	TArray<AActor*> IgnoreList;

};
