// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"

class APlayerCharacter;
class UInventoryComponent;
class UInventoryItemSlot;

UCLASS()
class UE5PROJECT_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION()
	void RefreshInventory();

	UPROPERTY(meta = (BindWidget))
	class UWrapBox* InventoryWrapBox;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WeightInfo;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CapacityInfo;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	UPROPERTY()
	UInventoryComponent* InventoryReference;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryItemSlot> InventorySlotClass;
	
protected:
	void SetInfoText() const;
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
