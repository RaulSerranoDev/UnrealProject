// Copyright RaulSerranoDev


#include "Game/MainGameModeBase.h"

#include "Kismet/GameplayStatics.h"

#include "UI/ViewModel/MVVM_LoadSlot.h"

void AMainGameModeBase::DeleteSlot(FString LoadSlotName, int32 SlotIndex)
{
}

void AMainGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
	const FString SlotName = Slot->GetLoadSlotName();
	const int32 SlotIndex = Slot->SlotIndex;

	//UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

ULoadScreenSaveGame* AMainGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	return nullptr;
}

void AMainGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	}

	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	//ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	//LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	//LoadScreenSaveGame->SaveSlotStatus = Taken;
	//LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
	//LoadScreenSaveGame->MapAssetName = LoadSlot->MapAssetName;
	//LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;

	//UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}
