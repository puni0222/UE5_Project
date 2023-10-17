// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"


class USoundBase;
class UBoxComponent;

UCLASS()
class UE5PROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();

	// Equip
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void DeactivateEmbers();
	void DisableSphereColilsion();
	void PlayEquipSound();
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	TArray<AActor*> IgnoreActors;

protected:
	virtual void BeginPlay() override;

	// Hit
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool ActorIsSameType(AActor* OtherActor);

	void ExecuteGetHit(FHitResult& BoxHit);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

private:
	
	// Damage
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;

	// BoxTrace
	void BoxTrace(FHitResult& BoxHit);
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTarceExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowBoxDebug = false;

	// Sound
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* EquipSound;
	
	// Components
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;
	
public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};
