// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROJECT_API UAttributeComponent : public UActorComponent
{  
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RegenStamina(float DeltaTime);

protected:
	virtual void BeginPlay() override;

private:
	
	// Current Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = "true"))
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = "true"))
	float MaxStamina;
		
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Xp;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxXp;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Souls;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DodgeCost = 14.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaRegenRate = 3.f;

public:
	
	void ReceiveDamage(float Damage);
	void UseStamina(float StaminaCost);
	float GetHealthPercent();
	float GetStaminaPercent();
	float GetXpPercent();
	bool IsAlive();
	void AddSouls(int32 NumberOfSouls);
	void AddGold(int32 AmountOfGold);
	void AddXp(float AmountOfXp);

	void AddHealth(float Value);
	void AddStamina(float Value);

	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE float GetStamina() const { return Stamina; }
	FORCEINLINE float GetHealth() const { return Health; }
};
