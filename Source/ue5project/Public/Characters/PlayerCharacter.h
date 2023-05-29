// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItem;
class UAnimMontage;
class USlashOverlay;
class ASoul;
class ATreasure;
class AHealthItem;
class UGroomComponent;
struct FItemInfoStruct;


UCLASS()
class UE5PROJECT_API APlayerCharacter : public ABaseCharacter , public IPickupInterface
{
	GENERATED_BODY()

public:

	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void SetOverlappingItem(class AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure) override;
	virtual void AddHealth(AHealthItem* HealthItem) override;

	void AddInventoryItem(FItemInfoStruct ItemData);

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;



	UPROPERTY(EditAnywhere, Category = InputAction)
	UInputMappingContext* SlashContext;
	
	UPROPERTY(EditAnywhere, Category = InputAction)
	UInputAction* MovementAction;
	
	UPROPERTY(EditAnywhere, Category = InputAction)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = InputAction)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = InputAction)
	UInputAction* EKeyAction;

	UPROPERTY(EditAnywhere, Category = InputAction)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = InputAction)
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, Category = InputAction)
	UInputAction* XKeyAction;

	UPROPERTY(EditAnywhere, Category = InputAction)
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = InputAction)
	UInputAction* IKeyAction;

	UPROPERTY(ReplicatedUsing = OnRep_InventoryItems, BlueprintReadWrite, Category = "Inventory")
	TArray<FItemInfoStruct> InventoryItems;
	
	UFUNCTION()
	void OnRep_InventoryItems();

	UFUNCTION()
	void OnRep_Stats();

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void AddItemAndUpdateInventoryWidget(FItemInfoStruct ItemData, const TArray<FItemInfoStruct>& CurrentInventory = TArray<FItemInfoStruct>());

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void UpdateInventoryWidget(const TArray<FItemInfoStruct>& NewInventoryItems);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Interact(FVector Start, FVector End);

	/* input */

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPressed();
	void Dodge();
	void XKeyPressed();
	void IKeyPressed();
	virtual void Attack() override;
	virtual void Jump() override;
	void Interact();
	void Interact(FVector Start, FVector End);

	/* Combat */
	void EquipWeapon(AWeapon* Weapon);
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;
	virtual bool CanAttack() override;
	bool CanDisarm();
	bool CanArm();
	void Disarm();
	void Arm();
	void PlayEquipMontage(const FName& SectionName);
	bool HasEnoughStamina();
	bool IsOccupied();
	virtual void Die_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseItem(TSubclassOf<AItem> ItemSubclass);

private:
	
	void InitializeSlashOverlay();
	void SetHUDHealth();

	/* Character Components */

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY()
	USlashOverlay* SlashOverlay;

public:
	
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};
