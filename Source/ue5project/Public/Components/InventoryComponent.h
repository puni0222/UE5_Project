// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);
class AItem;

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No item added"),
	IAR_PartialAmountItemAdded UMETA(DisplayName = "Partial item added"),
	IAR_AllItemAdded UMETA(DisplayName = "All item added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() : ActualAmountAdded(0), OperationResult(EItemAddResult::IAR_NoItemAdded), ResultMessage(FText::GetEmpty()) {};

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ActualAmountAdded;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult OperationResult;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;

	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	};
	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
	};
	static FItemAddResult AddedAll(const int32 AmountAdded, const FText& Message)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAdded;
		AddedAllResult.ResultMessage = Message;
		return AddedAllResult;
	};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	FOnInventoryUpdated OnInventoryUpdated;

	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleAddItem(AItem* InputItem);

	UFUNCTION(Category = "Inventory")
	AItem* FindMatchingItem(AItem* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	AItem* FindNextItemByID(AItem* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	AItem* FindNextPartialStack(AItem* ItemIn) const;	

	UFUNCTION(Category = "Inventory")
	void RemoveSingleInstanceOfItem(AItem* ItemToRemove);

	UFUNCTION(Category = "Inventory")
	int32 RemoveAmountOfItem(AItem* ItemIn, int32 AmountToRemove);

	UFUNCTION(Category = "Inventory")
	void SplitExistingStack(AItem* ItemIn, const int32 AmountToSplit);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<AItem>> InventoryArr;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float InventoryTotalWeight;

	UPROPERTY(EditInstanceOnly , Category = "Inventory")
	int32 InventorySlotsCapacity;

	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	float InventoryWeightCapacity;
	
	FItemAddResult HandleNonStackableItems(AItem* ItemIn);
	int32 HandleStackableItems(AItem* ItemIn, int32 RequestedAddAmount);
	int32 CalculateWeightAddAmount(AItem* ItemIn, int32 RequestedAddAmount);
	int32 CalculateNumberForFullStack(AItem* StackableItem, int32 InitialRequestedAddAmount);

	void AddNewItem(AItem* Item, const int32 AmountToAdd);

public:

	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetInventoryTotalWeight() const { return InventoryTotalWeight; };

	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetWeightCapacity() const { return InventoryWeightCapacity; };

	UFUNCTION(Category = "Inventory")
	FORCEINLINE int32 GetSlotsCapacity() const{ return InventorySlotsCapacity; };

	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<AItem*> GetInventoryArr() const { return InventoryArr; };

	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity) { InventorySlotsCapacity = NewSlotsCapacity; };

	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity) { InventoryWeightCapacity = NewWeightCapacity; };
};
