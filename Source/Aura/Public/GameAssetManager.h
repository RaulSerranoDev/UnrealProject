// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GameAssetManager.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UGameAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UGameAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;

};
