// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryItemSlot.h"
#include "UI/Inventory/InventoryTooltip.h"
#include "Item.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "UI/Inventory/DragItemVisual.h"
#include "UI/Inventory/ItemDragDropOperation.h"
//#include "Components/Widget.h"

void UInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (TooltipClass)
	{
		UInventoryTooltip* Tooltip = CreateWidget<UInventoryTooltip>(this, TooltipClass);
		Tooltip->InventorySlotBeingHovered = this;
		SetToolTip(Tooltip);
	}
}

void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemReference)
	{
		switch (ItemReference->ItemData.ItemQuality)
		{
		case EItemQuality::EIQ_Shoddy: 
			ItemBorder->SetBrushColor(FLinearColor::Gray);
			break;
		case EItemQuality::EIQ_Common:
			ItemBorder->SetBrushColor(FLinearColor::White);
			break;
		case EItemQuality::EIQ_Rare:
			ItemBorder->SetBrushColor(FLinearColor(0.0f,0.51f,0.169f));
			break;
		case EItemQuality::EIQ_Epic:
			ItemBorder->SetBrushColor(FLinearColor(0.0f,0.4f,0.75f));
			break;
		case EItemQuality::EIQ_Legendary:
			ItemBorder->SetBrushColor(FLinearColor(1.0f, 0.45f, 0.0f)); 
			break;
		default: 
			;
		}

		ItemIcon->SetBrushFromTexture(ItemReference->ItemData.AssetData.Icon);
		if (ItemReference->ItemData.NumericData.bIsStackable)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemReference->ItemQuantity));
		}
		else
		{
			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

FReply UInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	 
	return Reply.Unhandled();
}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (DragItemVisualClass)
	{
		const TObjectPtr<UDragItemVisual> DragVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
		DragVisual->ItemIcon->SetBrushFromTexture(ItemReference->ItemData.AssetData.Icon);
		DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());
		
		ItemReference->ItemData.NumericData.bIsStackable
			? DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->ItemQuantity))
			: DragVisual->ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);

		UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();
		DragItemOperation->SourceItem = ItemReference;
		DragItemOperation->SourceInventory = ItemReference->OwningInventory;

		DragItemOperation->DefaultDragVisual = DragVisual;
		DragItemOperation->Pivot = EDragPivot::TopLeft;

		OutOperation = DragItemOperation;
	}
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
