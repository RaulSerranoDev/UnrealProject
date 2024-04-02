// Copyright RaulSerranoDev


#include "Actor/EffectActor.h"

#include "AbilitySystemComponent.H"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"

AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void AEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
}

void AEffectActor::ApplyEffectsToTarget(AActor* TargetActor, TArray<TSubclassOf<UGameplayEffect>> GameplayEffectClasses)
{
	for (TSubclassOf<UGameplayEffect> GameplayEffectClass : GameplayEffectClasses)
	{
		ApplyEffectToTarget(TargetActor, GameplayEffectClass);
	}
}

void AEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectsToTarget(TargetActor, InstantGameplayEffectClasses);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectsToTarget(TargetActor, DurationGameplayEffectClasses);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectsToTarget(TargetActor, InfiniteGameplayEffectClasses);
	}
}

void AEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectsToTarget(TargetActor, InstantGameplayEffectClasses);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectsToTarget(TargetActor, DurationGameplayEffectClasses);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectsToTarget(TargetActor, InfiniteGameplayEffectClasses);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		for (TSubclassOf<UGameplayEffect> GameplayEffectClass : InfiniteGameplayEffectClasses)
		{
			TargetASC->RemoveActiveGameplayEffectBySourceEffect(GameplayEffectClass, TargetASC, 1);
		}
	}
}
