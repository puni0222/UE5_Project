// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryPanel.h"
#include "Characters/PlayerCharacter.h"
#include "Components/InventoryComponent.h"
#include "UI\Inventory/InventoryItemSlot.h"
#include "UI/Inventory/ItemDragDropOperation.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"


void UInventoryPanel::NativeOnInitialized()
{
	PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
		if (InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryPanel::RefreshInventory);
			SetInfoText();
		}
	}
}

void UInventoryPanel::SetInfoText() const
{
	const FString WeightInfoValue = { FString::SanitizeFloat(InventoryReference->GetInventoryTotalWeight()) + "/"
										+ FString::SanitizeFloat(InventoryReference->GetWeightCapacity()) };

	const FString CapacityInfoValue = { FString::FromInt(InventoryReference->GetInventoryArr().Num()) + "/"
										+ FString::FromInt(InventoryReference->GetSlotsCapacity()) };

	WeightInfo->SetText(FText::FromString(WeightInfoValue));
	CapacityInfo->SetText(FText::FromString(CapacityInfoValue));
}

void UInventoryPanel::RefreshInventory()
{
	if (InventoryReference && InventorySlotClass)
	{
		InventoryWrapBox->ClearChildren();

		for (AItem* const& InventoryItem : InventoryReference->GetInventoryArr())
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);

			InventoryWrapBox->AddChildToWrapBox(ItemSlot);
		}

		SetInfoText();
	}
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if (ItemDragDrop->SourceItem && InventoryReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detected an item drop on inventoryPanel"));

		return true;
	}
	return false;
}

