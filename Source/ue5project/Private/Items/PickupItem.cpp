// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickupItem.h"
#include "Characters\PlayerCharacter.h"
#include "Components\/InventoryComponent.h"

APickupItem::APickupItem()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMesh->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);

	ItemMesh->SetSimulatePhysics(true);
	
}

void APickupItem::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(AItem::StaticClass(), ItemQuantity);
}

void APickupItem::InitializePickup(const TSubclassOf<AItem> BaseClass, const int32 InQuantity)
{
	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const FItemData* NewItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<AItem>(this, BaseClass);

		ItemReference->ItemData.ID = NewItemData->ID;
		ItemReference->ItemData.ItemType = NewItemData->ItemType;
		ItemReference->ItemData.ItemQuality = NewItemData->ItemQuality;
		ItemReference->ItemData.NumericData = NewItemData->NumericData;
		ItemReference->ItemData.TextData = NewItemData->TextData;
		ItemReference->ItemData.AssetData = NewItemData->AssetData;
		
		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		ItemMesh->SetStaticMesh(NewItemData->AssetData.Mesh);

		UpdateInteractableData();
	}
}

void APickupItem::InitializeDrop(AItem* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->ItemData.NumericData.Weight = ItemToDrop->GetItemSingleWeight();
	ItemMesh->SetStaticMesh(ItemToDrop->ItemData.AssetData.Mesh);
	
	UpdateInteractableData();
}

void APickupItem::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->ItemData.TextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->ItemData.TextData.Name;
	InstanceInteractableData.Quantity = ItemReference->ItemQuantity;
	InteractableData = InstanceInteractableData;
	
}

void APickupItem::BeginFocus()
{
	if (ItemMesh)
	{
		ItemMesh->SetRenderCustomDepth(true);
	}
}

void APickupItem::EndFocus()
{
	if (ItemMesh)
	{
		ItemMesh->SetRenderCustomDepth(false);
	}
}

void APickupItem::Interact(APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		TakePickup(PlayerCharacter);
	}
}

void APickupItem::TakePickup(const APlayerCharacter* Taker)
{
	if (!IsPendingKillPending())
	{
		if (ItemReference)
		{
			if (UInventoryComponent* PlayerInventory = Taker->GetInventory())
			{
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

				switch (AddResult.OperationResult)
				{
				case EItemAddResult::IAR_NoItemAdded:
					break;
					
				case EItemAddResult::IAR_PartialAmountItemAdded:
					UpdateInteractableData();
					Taker->UpdateInteractionWidget();
					break;

				case EItemAddResult::IAR_AllItemAdded:
					Destroy();
					break;
				}

				UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Player inventorycomponent is null"));
			}

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Pickup item reference is null"));
		}
	}
}


// 에디터에서 DesiredItemID를 바꾸면 해당 DataTable을 에디터에서 즉시 반영.
void APickupItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickupItem, DesiredItemID))
	{
		if (ItemDataTable)
		{
			if (const FItemData* NewItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString()))
			{
				ItemMesh->SetStaticMesh(NewItemData->AssetData.Mesh);
			}
		}
	}
}
