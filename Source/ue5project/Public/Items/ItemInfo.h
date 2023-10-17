#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemInfo.generated.h"

UENUM(BlueprintType)
enum class EItemQuality : uint8
{
	EIQ_Shoddy UMETA(DisplayName = "Shoddy"),
	EIQ_Common UMETA(DisplayName = "Common"),
	EIQ_Rare UMETA(DisplayName = "Rare"),
	EIQ_Epic UMETA(DisplayName = "Epic"),
	EIQ_Legendary UMETA(DisplayName = "Legendary")
};

UENUM(BlueprintType)
enum class EItemName : uint8
{
	EIN_None UMETA(DisplayName = "None"),
	EIN_HealthPotion UMETA(DisplayName = "Health Potion"),
	EIN_StaminaPotion UMETA(DisplayName = "StaminaPotion"),
	EIN_Sword UMETA(DisplayName = "Sword"),
	EIN_Shield UMETA(DisplayName = "Shield")
};

UENUM(BlueprintType)
enum class EItemTypes : uint8
{
	EIT_None UMETA(DisplayName = "None"),
	EIT_HealthItem UMETA(DisplayName = "HealthItem"),
	EIT_StaminaItem UMETA(DisplayName = "StaminaItem"),
	EIT_Weapon UMETA(DisplayName = "Weapon"),
	EIT_Shield UMETA(DisplayName = "Shield"),
	EIT_Armor UMETA(DisplayName = "Armor"),
	EIT_Consumable UMETA(DisplayName = "Consumable"),
	//EIT_KeyItem UMETA(DisplayName = "KeyItem"),
	//EIT_Resources UMETA(DisplayName = "Resources"),
	EIT_Spell UMETA(DisplayName = "Spell"),
	EIT_Quest UMETA(DisplayName = "Quest"),
	EIT_Mundane UMETA(DisplayName = "Mundane")
};

USTRUCT(BlueprintType)
struct FItemStatistics
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float ArmorRating;

	UPROPERTY(EditAnywhere)
	float DamageValue;

	UPROPERTY(EditAnywhere)
	float RestorationAmount;

	UPROPERTY(EditAnywhere)
	float SellValue;
};

USTRUCT(BlueprintType)
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FText InteractionText;

	UPROPERTY(EditAnywhere)
	FText UsageText;
};

USTRUCT(BlueprintType)
struct FItemNumericData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float Weight;

	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere)
	bool bIsStackable;
};

USTRUCT(BlueprintType)
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemTypes ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;

	UPROPERTY(VisibleAnywhere, Category = "Item Data")
	FItemStatistics ItemStatistics;

	UPROPERTY(VisibleAnywhere, Category = "Item Data")
	FItemTextData TextData;

	UPROPERTY(VisibleAnywhere, Category = "Item Data")
	FItemNumericData NumericData;

	UPROPERTY(VisibleAnywhere, Category = "Item Data")
	FItemAssetData AssetData;
	
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

