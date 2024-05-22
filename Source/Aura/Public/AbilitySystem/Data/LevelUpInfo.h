// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FGameLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 XPRequirement = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointReward = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointReward = 1;
};

/**
 *
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	int32 FindLevelForXP(const int32& XP, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, Category = "LevelUpInformation")
	TArray<FGameLevelUpInfo> LevelUpInfo;
};
