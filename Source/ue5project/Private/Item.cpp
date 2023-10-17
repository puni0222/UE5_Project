// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ItemInfoComponent.h"
#include "Components/InventoryComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Interfaces/PickupInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/PlayerCharacter.h"


AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	//ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	ItemEffect->SetupAttachment(GetRootComponent());
	
	bReplicates = true;

	bIsCopy = false;
	bIsPickup = false;
}

void AItem::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}

AItem* AItem::CreateItemCopy()
{
	AItem* ItemCopy = NewObject<AItem>(StaticClass());

	ItemCopy->ItemData.ID = this->ItemData.ID;
	ItemCopy->ItemQuantity = this->ItemQuantity;
	ItemCopy->ItemData.ItemQuality = this->ItemData.ItemQuality;
	ItemCopy->ItemData.ItemType = this->ItemData.ItemType;
	ItemCopy->ItemData.TextData = this->ItemData.TextData;
	ItemCopy->ItemData.NumericData = this->ItemData.NumericData;
	ItemCopy->ItemData.ItemStatistics = this->ItemData.ItemStatistics;
	ItemCopy->ItemData.AssetData = this->ItemData.AssetData;
	ItemCopy->bIsCopy = true;

	return ItemCopy;
}

void AItem::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != ItemQuantity)
	{
		ItemQuantity = FMath::Clamp(NewQuantity, 0,
			this->ItemData.NumericData.bIsStackable ? this->ItemData.NumericData.MaxStackSize : 1);

		if (this->OwningInventory)
		{
			if (this->ItemQuantity <= 0)
			{
				this->OwningInventory->RemoveSingleInstanceOfItem(this);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ItemBase OwningInventory was null"));
		}
	}
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	
	InteractableData = InstanceInteractableData;
}

void AItem::BeginFocus()
{
}

void AItem::EndFocus()
{
}

void AItem::BeginInteract()
{
}

void AItem::EndInteract()
{
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransfromedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItem(nullptr);
	}
}

void AItem::SpawnPickupSystem()
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickupEffect,
			GetActorLocation()
		);
	}
}

void AItem::SpawnPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			PickupSound,
			GetActorLocation()
		);
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;
	
	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
}

