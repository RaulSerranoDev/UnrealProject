// Copyright RaulSerranoDev


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitSlot()
{
	// TODO: Check slot status based on loaded data
	//const int32 WidgetSwitcherIndex = SlotStatus.GetValue();
	//SetWidgetSwitcherIndex.Broadcast(WidgetSwitcherIndex);
	SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadSlot::SetPlayerName(FString InPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

void UMVVM_LoadSlot::SetMapName(FString InMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);
}

void UMVVM_LoadSlot::SetPlayerLevel(int32 InLevel)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerLevel, InLevel);
}

void UMVVM_LoadSlot::SetLoadSlotName(FString InLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}
