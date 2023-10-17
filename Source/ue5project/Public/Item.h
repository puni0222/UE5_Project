// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractableInterface.h"
#include "Items/ItemInfo.h"
#include "Item.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UItemInfoComponent;
struct FInteractableData;
class UInventoryComponent;

UCLASS()
class UE5PROJECT_API AItem : public AActor , public IInteractableInterface
{
	GENERATED_BODY()
	
public:
	
	// Components
	UPROPERTY()
	UInventoryComponent* OwningInventory;

	// Item Data
	UPROPERTY(EditAnywhere, Category = "Item Initialization")
	FItemData ItemData;

	UPROPERTY(EditAnywhere, Category = "Item Initialization")
	int32 ItemQuantity;

	UPROPERTY(EditAnywhere, Category = "Item Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, Category = "Item Initialization")
	FName DesiredItemID;

	bool bIsCopy;
	bool bIsPickup;	

public:	

	AItem();
	virtual void Tick(float DeltaTime) override;
	
	// Inventory
	void ResetItemFlags();
	AItem* CreateItemCopy();
	void SetQuantity(const int32 NewQuantity);
	
protected:

	virtual void BeginPlay() override;

	// Interaction
	UPROPERTY(EditInstanceOnly , Category = "Item")
	FInteractableData InstanceInteractableData;
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	bool operator==(const FName& OtherID) const { return this->ItemData.ID == OtherID; }

	// Hovering
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Amplitude = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float TimeConstant = 5.f;

	UFUNCTION(BlueprintPure)
	float TransformedSin();

	UFUNCTION(BlueprintPure)
	float TransfromedCos();

	template<typename T>
	T Avg(T Frist, T Second);


	// Overlap
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// Sound , Particle
	virtual void SpawnPickupSystem();
	virtual void SpawnPickupSound();

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* ItemEffect;
	
	// Sound
	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;

	// Effect
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* PickupEffect;

	// ItemName
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemName ItemName = EItemName::EIN_None;

	// State
	EItemState ItemState = EItemState::EIS_Hovering;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RunningTime;

public:

	FORCEINLINE float GetItemStackWeight() const { return ItemQuantity * ItemData.NumericData.Weight; };
	FORCEINLINE float GetItemSingleWeight() const { return ItemData.NumericData.Weight; };
	FORCEINLINE bool IsFullItemStack() const { return ItemQuantity == ItemData.NumericData.MaxStackSize; };
};

template<typename T>
inline T AItem::Avg(T Frist, T Second)
{
	return (First + Second) / 2;
}
