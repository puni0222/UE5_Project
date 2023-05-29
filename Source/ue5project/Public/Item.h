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

UCLASS()
class UE5PROJECT_API AItem : public AActor , public IInteractableInterface
{
	GENERATED_BODY()
	
public:	

	AItem();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(class APlayerCharacter* Character) override;
	virtual void Use(APlayerCharacter* Character) override;

protected:

	virtual void BeginPlay() override;

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

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	virtual void SpawnPickupSystem();
	virtual void SpawnPickupSound();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	EItemState ItemState = EItemState::EIS_Hovering;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* ItemEffect;
	
	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* PickupEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UItemInfoComponent* ItemInfoComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemName ItemName = EItemName::EIN_None;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RunningTime;

};

template<typename T>
inline T AItem::Avg(T Frist, T Second)
{
	return (First + Second) / 2;
}
