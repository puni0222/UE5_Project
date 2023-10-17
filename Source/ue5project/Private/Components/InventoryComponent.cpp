// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Item.h"
#include "Kismet/GameplayStatics.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

AItem* UInventoryComponent::FindMatchingItem(AItem* ItemIn) const
{
	if (ItemIn)
	{
		if (InventoryArr.Contains(ItemIn))
		{
			return ItemIn;
		}
	}

	return nullptr;
}

AItem* UInventoryComponent::FindNextItemByID(AItem* ItemIn) const
{
	if (ItemIn)
	{
		//TObjectPtr 자체가 포인터이기 때문에 Result는 이중 포인터가 되고, 반환값을 포인터로 반환해야함.
		if (const TArray<TObjectPtr<AItem>>::ElementType* Result = InventoryArr.FindByKey(ItemIn))
		{
			return *Result;
		}
	}

	return nullptr;
}

AItem* UInventoryComponent::FindNextPartialStack(AItem* ItemIn) const
{
	if (const TArray<TObjectPtr<AItem>>::ElementType* Result = 
		InventoryArr.FindByPredicate([&ItemIn](const AItem* InventoryItem)
			{
				return InventoryItem->ItemData.ID == ItemIn->ItemData.ID && !InventoryItem->IsFullItemStack();
			}))
	{
		return *Result;
	}

	return nullptr;
}

int32 UInventoryComponent::CalculateWeightAddAmount(AItem* ItemIn, int32 RequestedAddAmount)
{
	//FloorToInt()는 내림 값을 반환.
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight());
	if (WeightMaxAddAmount > RequestedAddAmount)
	{
		return RequestedAddAmount;
	}

	return WeightMaxAddAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStack(AItem* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->ItemData.NumericData.MaxStackSize - StackableItem->ItemQuantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UInventoryComponent::RemoveSingleInstanceOfItem(AItem* ItemToRemove)
{
	InventoryArr.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::RemoveAmountOfItem(AItem* ItemIn, int32 AmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(AmountToRemove, ItemIn->ItemQuantity);
	
	ItemIn->SetQuantity(ItemIn->ItemQuantity - ActualAmountToRemove);
	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();
	OnInventoryUpdated.Broadcast();

	return ActualAmountToRemove;
}

void UInventoryComponent::SplitExistingStack(AItem* ItemIn, const int32 AmountToSplit)
{
	if (!(InventoryArr.Num() + 1 > InventorySlotsCapacity))
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(AItem* ItemIn)
{
	if (FMath::IsNearlyZero(ItemIn->GetItemSingleWeight()) || ItemIn->GetItemSingleWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. Item has invalid weight value."),
			ItemIn->ItemData.TextData.Name));
	}

	if (InventoryTotalWeight + ItemIn->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. Item would overflow weight limit."), 
			ItemIn->ItemData.TextData.Name));
	}

	if (InventoryArr.Num() + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. All inventory  slots are full"), 
			ItemIn->ItemData.TextData.Name));
	}

	AddNewItem(ItemIn, 1);

	return FItemAddResult::AddedAll(1, FText::Format(
		FText::FromString("Successfully added a single {0} to the inventory."),
		ItemIn->ItemData.TextData.Name));
	
}

int32 UInventoryComponent::HandleStackableItems(AItem* ItemIn, int32 RequestedAddAmount)
{
	if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(ItemIn->GetItemStackWeight()))
	{
		return 0;
	}
	
	int32 AmountToDistribute = RequestedAddAmount;
	// 아이템이 이미 인벤토리에 있고 풀스택이 아닌지 확인.
	AItem* ExistingItemStack = FindNextPartialStack(ItemIn);

	// input item이 기존에 인벤토리에 있다면
	while (ExistingItemStack)
	{
		// 다음 풀스택까지 필요한 개수를 계산.
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);
		// 인벤토리 무게 제한까지 추가 가능한 스택의 수 계산
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack);

		// 무게제한이 남아있다면
		if (WeightLimitAddAmount > 0)
		{
			// 아이템 스택 개수와 인벤토리의 현재 무게를 조정한다.
			ExistingItemStack->SetQuantity(ExistingItemStack->ItemQuantity + WeightLimitAddAmount);
			InventoryTotalWeight += (ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount);

			// 분배할 개수를 계산한다.
			AmountToDistribute -= WeightLimitAddAmount;
			ItemIn->SetQuantity(AmountToDistribute);

			// 인벤토리 현재 중량이 최대 중량 이상이라면 return
			if (InventoryTotalWeight >= InventoryWeightCapacity)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
		} 
		else if (WeightLimitAddAmount <= 0)  // 무게제한이 남지 않았다면
		{
			if (AmountToDistribute != RequestedAddAmount)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}

			return 0;
		}

		if (AmountToDistribute <= 0)
		{
			// 모든 스택이 분배되었다면 return
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}

		// 또 다른 부분스택이 남았는지 확인.
		ExistingItemStack = FindNextPartialStack(ItemIn);
	}

	// 부분스택을 찾지못하면, 새로운 아이템 슬롯에 스택을 채울 수 있는지 체크
	if (InventoryArr.Num() + 1 <= InventorySlotsCapacity)
	{
		// 인벤토리 무게 총량에 채울 수 있는 만큼 남은 아이템수량 추가를 시도합니다.
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn, AmountToDistribute);
		
		if (WeightLimitAddAmount > 0)
		{
			// 분배할 아이템이 남아있지만 무게 제한에 도달했을 경우
			if (WeightLimitAddAmount < AmountToDistribute)
			{
				// 남은 수량에 담을 수 있을만큼 수량을 조정 후 추가합니다.
				AmountToDistribute -= WeightLimitAddAmount;
				ItemIn->SetQuantity(AmountToDistribute);
				AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);
				return RequestedAddAmount - AmountToDistribute;
			}

			// 그렇지 않을 경우 아이템을 추가합니다.
			AddNewItem(ItemIn, AmountToDistribute);
			return RequestedAddAmount;
		}
	}

	OnInventoryUpdated.Broadcast();
	return RequestedAddAmount - AmountToDistribute;
}

FItemAddResult UInventoryComponent::HandleAddItem(AItem* InputItem)
{
	if (GetOwner())
	{
		const int32 AddAmount = InputItem->ItemQuantity;

		if (!InputItem->ItemData.NumericData.bIsStackable)
		{
			return HandleNonStackableItems(InputItem);
		}

		const int32 StackableAddAmount = HandleStackableItems(InputItem, AddAmount);

		if (StackableAddAmount == AddAmount)
		{
			return FItemAddResult::AddedAll(AddAmount, FText::Format(
				FText::FromString("Successfully added {0} {1} to the inventory."), 
				AddAmount, InputItem->ItemData.TextData.Name));
		}

		if (StackableAddAmount < AddAmount && StackableAddAmount > 0)
		{
			return FItemAddResult::AddedPartial(StackableAddAmount, FText::Format(
				FText::FromString("Partial amount of {0} added to the inventory. Number added = {1}"), 
				InputItem->ItemData.TextData.Name, StackableAddAmount));
		}

		if (StackableAddAmount <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(
				FText::FromString("Couldn't add {0} to the inventory. No remaining inventory slots, or invalid item."), 
				InputItem->ItemData.TextData.Name));
		}
	}

	
	return FItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. GetOwner() check somehow failed."));
}

void UInventoryComponent::AddNewItem(AItem* Item, const int32 AmountToAdd)
{
	AItem* NewItem;
	
	if (Item->bIsCopy || Item->bIsPickup)
	{
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	InventoryArr.Add(NewItem);
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();
}