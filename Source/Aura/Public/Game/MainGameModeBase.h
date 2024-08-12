// Copyright RaulSerranoDev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameModeBase.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;
class UMVVM_LoadSlot;
class ULoadScreenSaveGame;
class USaveGame;

/**
 *
 */
UCLASS()
class AURA_API AMainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	static void DeleteSlot(FString LoadSlotName, int32 SlotIndex);
	void TravelToMap(UMVVM_LoadSlot* Slot);
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Load/Save")
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

};
