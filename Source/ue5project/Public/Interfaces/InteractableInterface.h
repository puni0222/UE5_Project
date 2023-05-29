// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class APlayerCharacter;

class UE5PROJECT_API IInteractableInterface
{
	GENERATED_BODY()

public:
	virtual void Interact(APlayerCharacter* Character) = 0;
	virtual void Use(APlayerCharacter* Character) {}
};
