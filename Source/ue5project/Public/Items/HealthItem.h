// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "HealthItem.generated.h"

class APlayerCharacter;

UCLASS()
class UE5PROJECT_API AHealthItem : public AItem
{
	GENERATED_BODY()

public:

	AHealthItem();
	virtual void Use(APlayerCharacter* Character) override;
	
protected:

	UPROPERTY(EditAnywhere)
	float HealthValue;

public:
	FORCEINLINE float GetHealth() const { return HealthValue; }
};
