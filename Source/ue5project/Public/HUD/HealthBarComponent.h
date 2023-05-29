// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	
	void SetHealthPercent(float Precent);

private:

	UPROPERTY()
	class UHealthBar* HealthBarWidget;
};
