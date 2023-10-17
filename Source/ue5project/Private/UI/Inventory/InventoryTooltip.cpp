// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryTooltip.h"
#include "UI/Inventory/InventoryItemSlot.h"
#include "Item.h"

void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	AItem* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();
	
	switch (ItemBeingHovered->ItemData.ItemType)
	{
	case EItemTypes::EIT_Armor:
		break;
	case EItemTypes::EIT_Weapon:
		break;
	case EItemTypes::EIT_Shield:
		break;
	case EItemTypes::EIT_Spell:
		break;
	case EItemTypes::EIT_Consumable:
		ItemType->SetText(FText::FromString("Consumable"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemTypes::EIT_Quest:
		break;
	case EItemTypes::EIT_Mundane:
		ItemType->SetText(FText::FromString("Mundane Item"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
		break;

	default:
		break;
	}

	ItemName->SetText(ItemBeingHovered->ItemData.TextData.Name);
	DamageValue->SetText(FText::AsNumber(ItemBeingHovered->ItemData.ItemStatistics.DamageValue));
	ArmorRating->SetText(FText::AsNumber(ItemBeingHovered->ItemData.ItemStatistics.ArmorRating));
	UsageText->SetText(ItemBeingHovered->ItemData.TextData.UsageText);
	ItemDescription->SetText(ItemBeingHovered->ItemData.TextData.Description);

	const FString WeightInfo = { "Weight : " + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight()) };
	StackWeight->SetText(FText::FromString(WeightInfo));

	if (ItemBeingHovered->ItemData.NumericData.bIsStackable)
	{
		const FString StackInfo = { "Max stack size : " + FString::FromInt(ItemBeingHovered->ItemData.NumericData.MaxStackSize) };
		MaxStackSize->SetText(FText::FromString(StackInfo));
	}
	else
	{
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}
}
