// Copyright RaulSerranoDev


#include "AbilitySystem/Abilities/FireBlast.h"

TArray<AFireBallProjectile*> UFireBlast::SpawnFireBalls()
{
	return TArray<AFireBallProjectile*>();
}

int32 UFireBlast::GetNumFireBalls(int32 Level) const
{
	return NumFireBalls;
}
