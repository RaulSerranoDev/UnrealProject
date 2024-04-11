// Copyright RaulSerranoDev


#include "GameAssetManager.h"

#include "GameGameplayTags.h"

UGameAssetManager& UGameAssetManager::Get()
{
	check(GEngine);

	UGameAssetManager* GameAssetManager = Cast<UGameAssetManager>(GEngine->AssetManager);
	return *GameAssetManager;
}

void UGameAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FGameGameplayTags::InitalizeNativeGameplayTags();
}
