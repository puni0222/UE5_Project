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
		//TObjectPtr ��ü�� �������̱� ������ Result�� ���� �����Ͱ� �ǰ�, ��ȯ���� �����ͷ� ��ȯ�ؾ���.
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
	//FloorToInt()�� ���� ���� ��ȯ.
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
	// �������� �̹� �κ��丮�� �ְ� Ǯ������ �ƴ��� Ȯ��.
	AItem* ExistingItemStack = FindNextPartialStack(ItemIn);

	// input item�� ������ �κ��丮�� �ִٸ�
	while (ExistingItemStack)
	{
		// ���� Ǯ���ñ��� �ʿ��� ������ ���.
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);
		// �κ��丮 ���� ���ѱ��� �߰� ������ ������ �� ���
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack);

		// ���������� �����ִٸ�
		if (WeightLimitAddAmount > 0)
		{
			// ������ ���� ������ �κ��丮�� ���� ���Ը� �����Ѵ�.
			ExistingItemStack->SetQuantity(ExistingItemStack->ItemQuantity + WeightLimitAddAmount);
			InventoryTotalWeight += (ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount);

			// �й��� ������ ����Ѵ�.
			AmountToDistribute -= WeightLimitAddAmount;
			ItemIn->SetQuantity(AmountToDistribute);

			// �κ��丮 ���� �߷��� �ִ� �߷� �̻��̶�� return
			if (InventoryTotalWeight >= InventoryWeightCapacity)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
		} 
		else if (WeightLimitAddAmount <= 0)  // ���������� ���� �ʾҴٸ�
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
			// ��� ������ �й�Ǿ��ٸ� return
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}

		// �� �ٸ� �κн����� ���Ҵ��� Ȯ��.
		ExistingItemStack = FindNextPartialStack(ItemIn);
	}

	// �κн����� ã�����ϸ�, ���ο� ������ ���Կ� ������ ä�� �� �ִ��� üũ
	if (InventoryArr.Num() + 1 <= InventorySlotsCapacity)
	{
		// �κ��丮 ���� �ѷ��� ä�� �� �ִ� ��ŭ ���� �����ۼ��� �߰��� �õ��մϴ�.
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn, AmountToDistribute);
		
		if (WeightLimitAddAmount > 0)
		{
			// �й��� �������� ���������� ���� ���ѿ� �������� ���
			if (WeightLimitAddAmount < AmountToDistribute)
			{
				// ���� ������ ���� �� ������ŭ ������ ���� �� �߰��մϴ�.
				AmountToDistribute -= WeightLimitAddAmount;
				ItemIn->SetQuantity(AmountToDistribute);
				AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);
				return RequestedAddAmount - AmountToDistribute;
			}

			// �׷��� ���� ��� �������� �߰��մϴ�.
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