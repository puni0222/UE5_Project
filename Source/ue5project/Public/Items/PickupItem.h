// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "PickupItem.generated.h"

class UDataTable;
class AItem;

UCLASS()
class UE5PROJECT_API APickupItem : public AItem
{
	GENERATED_BODY()
		
public:
	APickupItem();

	void InitializePickup(const TSubclassOf<AItem> BaseClass, const int32 InQuantity);
	void InitializeDrop(AItem* ItemToDrop, const int32 InQuantity);
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;

protected:

	virtual void BeginPlay() override;
	virtual void Interact(APlayerCharacter* PlayerCharacter) override;
	void UpdateInteractableData();
	void TakePickup(const APlayerCharacter* Taker);
	
	UPROPERTY(EditInstanceOnly, Category = "Pick up | ItemReference")
	AItem* ItemReference;

public:

	FORCEINLINE AItem* GetItemData() { return ItemReference; }

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
