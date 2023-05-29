// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ItemInfoComponent.h"
#include "Item.h"

UItemInfoComponent::UItemInfoComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	ItemData.ItemClass = TSubclassOf<UObject>(GetOuter()->GetClass());
}

void UItemInfoComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UItemInfoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UItemInfoComponent::InitializeComponent()
{
	Super::InitializeComponent();
}
