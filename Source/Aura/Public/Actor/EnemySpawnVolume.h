// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "EnemySpawnVolume.generated.h"

class AEnemySpawnPoint;
class UBoxComponent;

UCLASS()
class AURA_API AEnemySpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()

public:
	AEnemySpawnVolume();

	/* Save Interface */
	virtual void LoadActor_Implementation() override;
	/* end Save Interface */

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;

protected:
	UPROPERTY(EditAnywhere)
	TArray<AEnemySpawnPoint*> SpawnPoints;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;

};
