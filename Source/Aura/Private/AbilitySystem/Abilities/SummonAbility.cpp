// Copyright RaulSerranoDev


#include "AbilitySystem/Abilities/SummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> USummonAbility::GetSpawnLocations(bool bShowDebug)
{
	if (NumMinions <= 0) TArray<FVector>();;

	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpreadAngle / NumMinions;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpreadAngle / 2.f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpreadAngle / 2.f, FVector::UpVector);

	if (bShowDebug)
	{
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + LeftOfSpread * MaxSpawnDistance, 4.f, FLinearColor::Blue, 3.f);
		DrawDebugSphere(GetWorld(), Location + LeftOfSpread * MinSpawnDistance, 10.f, 12, FColor::Blue, false, 3.f);
		DrawDebugSphere(GetWorld(), Location + LeftOfSpread * MaxSpawnDistance, 10.f, 12, FColor::Blue, false, 3.f);

		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + RightOfSpread * MaxSpawnDistance, 4.f, FLinearColor::Red, 3.f);
		DrawDebugSphere(GetWorld(), Location + RightOfSpread * MinSpawnDistance, 10.f, 12, FColor::Red, false, 3.f);
		DrawDebugSphere(GetWorld(), Location + RightOfSpread * MaxSpawnDistance, 10.f, 12, FColor::Red, false, 3.f);
	}

	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i + DeltaSpread / 2.f, FVector::UpVector);

		FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 400.f), ChosenSpawnLocation - FVector(0.f, 0.f, 400.f), ECollisionChannel::ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
		}
		SpawnLocations.Add(ChosenSpawnLocation);

		if (bShowDebug)
		{
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + Direction * MaxSpawnDistance, 4.f, FLinearColor::Green, 3.f);
			DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistance, 5.f, 12, FColor::Green, false, 3.f);
			DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistance, 5.f, 12, FColor::Green, false, 3.f);

			DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 18.f, 12, FColor::Cyan, false, 3.f);
		}
	}

	return SpawnLocations;
}
