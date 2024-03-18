// Copyright RaulSerranoDev


#include "Actor/EffectActor.h"

#include "Components/SphereComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/GameAttributeSet.h"

AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void AEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO: Change this to apply a Gameplay Effect. For now, using const_cast as a hack!

	IAbilitySystemInterface* ASCInterface = Cast< IAbilitySystemInterface>(OtherActor);
	if (!ASCInterface)
		return;

	const UGameAttributeSet* GameAttributeSet = Cast<UGameAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UGameAttributeSet::StaticClass()));
	UGameAttributeSet* MutableGameAttributeSet = const_cast<UGameAttributeSet*>(GameAttributeSet);
	MutableGameAttributeSet->SetHealth(GameAttributeSet->GetHealth() + 25.f);
	Destroy();
}

void AEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AEffectActor::EndOverlap);
}
