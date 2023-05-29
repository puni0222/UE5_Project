// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Items/ItemInfo.h"
#include "Engine/DataTable.h"
#include "MyGameInstance.generated.h"



UCLASS()
class UE5PROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMyGameInstance();

	virtual void Init() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	class UDataTable* ItemInfoDataTable;
};
