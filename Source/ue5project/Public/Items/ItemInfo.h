#pragma once

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
	EIT_Sword UMETA(DisplayName = "Sword"),
	EIT_Shield UMETA(DisplayName = "Shield"),
	EIT_KeyItem UMETA(DisplayName = "KeyItem"),
	EIT_Resources UMETA(DisplayName = "Resources")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

