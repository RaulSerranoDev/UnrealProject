// Copyright RaulSerranoDev


#include "Actor/EnemySpawnVolume.h"

#include "Components/BoxComponent.h"

#include "Actor/EnemySpawnPoint.h"
#include "Aura/Aura.h"
#include "Interaction/PlayerInterface.h"

AEnemySpawnVolume::AEnemySpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(Box);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(ECC_WorldStatic);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AEnemySpawnVolume::LoadActor_Implementation()
{
	if (bReached)
	{
		Destroy();
	}
}

void AEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxOverlap);
}

void AEnemySpawnVolume::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UPlayerInterface>()) return;

	bReached = true;
	for (AEnemySpawnPoint* Point : SpawnPoints)
	{
		if (IsValid(Point))
		{
			Point->SpawnEnemy();
		}
	}
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
