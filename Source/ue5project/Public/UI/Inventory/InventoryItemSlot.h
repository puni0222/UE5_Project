// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemSlot.generated.h"

class AItem;
class UDragItemVisual;
class UInventoryTooltip;
class UBorder;
class UImage;
class UTextBlock;

UCLASS()
class UE5PROJECT_API UInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	

protected:

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	AItem* ItemReference;

	UPROPERTY(EditDefaultsOnly, Category = "Inventroy Slot")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventroy Slot")
	TSubclassOf<UInventoryTooltip> TooltipClass;

	UPROPERTY(VisibleAnywhere, Category = "Inventroy Slot", meta = (BindWidget))
	UBorder* ItemBorder;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventroy Slot", meta = (BindWidget))
	UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, Category = "Inventroy Slot", meta = (BindWidget))
	UTextBlock* ItemQuantity;
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	FORCEINLINE void SetItemReference(AItem* ItemIn) { ItemReference = ItemIn; };
	FORCEINLINE AItem* GetItemReference() const { return ItemReference; };
};
