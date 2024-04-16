// Copyright RaulSerranoDev


#include "Actor/SelectableActor.h"

// Sets default values
ASelectableActor::ASelectableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASelectableActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASelectableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASelectableActor::Select()
{
	UE_LOG(LogTemp, Display, TEXT("Select Actor: %s"), *GetName());
}

