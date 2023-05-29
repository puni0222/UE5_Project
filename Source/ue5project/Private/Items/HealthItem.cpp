// Fill out your copyright notice in the Description page of Project Settings.


#include "items/HealthItem.h"
#include "Characters/PlayerCharacter.h"
#include "Components/AttributeComponent.h"
#include "Components/ItemInfoComponent.h"

AHealthItem::AHealthItem()
{
	HealthValue = 10.f;
}

void AHealthItem::Use(APlayerCharacter* Character)
{
	if (Character)
	{
		Character->AddHealth(this);
	}
}
