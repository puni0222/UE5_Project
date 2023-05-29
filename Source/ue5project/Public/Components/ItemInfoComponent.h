// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/ItemInfo.h"
#include "Engine/DataTable.h"
#include "ItemInfoComponent.generated.h"

USTRUCT(BlueprintType)
struct FItemInfoStruct : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AItem> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* ItemTexture;

	UPROPERTY(BlueprintReadWrite)
		int32 CurrentStack = 1;

	UPROPERTY(BlueprintReadWrite)
		int32 MaxStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ItemIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Potency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SellValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EItemTypes ItemType = EItemTypes::EIT_None;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROJECT_API UItemInfoComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemInfoComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override; 


private:
	
	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	FItemInfoStruct ItemData;
		
public:

	FORCEINLINE FItemInfoStruct GetItemData() const { return ItemData; }

	
};
