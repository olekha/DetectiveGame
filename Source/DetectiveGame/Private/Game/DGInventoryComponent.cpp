
#include "Game/DGInventoryComponent.h"
#include "DGInventoryItem.h"

UDGInventoryComponent::UDGInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UDGInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

ADGInventoryItem* UDGInventoryComponent::GetItem(uint16 index) const
{
	if (Inventory.IsValidIndex(index))
	{
		return Inventory[index];
	}

	return nullptr;
}

bool UDGInventoryComponent::HasAnItemInIventory(ADGInventoryItem* Item) const
{
	return Inventory.Find(Item) != INDEX_NONE;
}

void UDGInventoryComponent::AddToInventory(ADGInventoryItem* Item)
{
	if (!HasAnItemInIventory(Item))
	{
		Inventory.Add(Item);
	}
}

void UDGInventoryComponent::RemoveFromInventory(ADGInventoryItem* Item)
{
	Inventory.Remove(Item);
}
