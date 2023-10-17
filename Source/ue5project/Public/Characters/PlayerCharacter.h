// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "Interfaces/InteractableInterface.h"
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
class AMyHUD;
class UInventoryComponent;


USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()
	
	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f) {};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

UCLASS()
class UE5PROJECT_API APlayerCharacter : public ABaseCharacter , public IPickupInterface
{
	GENERATED_BODY()

public:

	APlayerCharacter();

	// AActor
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	// HitInterface
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	// PickupInterface
	virtual void SetOverlappingItem(class AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure) override;
	virtual void AddHealth(AHealthItem* HealthItem) override;

	// Inventory
	void UpdateInteractionWidget() const;
	void DropItem(AItem* ItemToDrop, const int32 QuantityToDrop);

protected:
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;

	// UI
	UPROPERTY()
	AMyHUD* MyHUD;

	// Interaction
	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractableInterface> TargetInteractable;

	float InteractionCheckFrequency;

	float InteractionCheckDistance;
	
	FTimerHandle TimerHandle_Interaction;

	FInteractionData InteractionData;

	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();

	// Input
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

	UPROPERTY(EditAnywhere, Category = InputAction)
	UInputAction* BeginInteractAction;

	UPROPERTY(EditAnywhere, Category = InputAction)
	UInputAction* EndInteractAction;
	
	// Input Action
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPressed();
	void Dodge();
	void XKeyPressed();
	void IKeyPressed();
	void ToggleMenu();
	virtual void Attack() override;
	virtual void Jump() override;

	// Combat
	virtual void Die_Implementation() override;
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;
	virtual bool CanAttack() override;
	void EquipWeapon(AWeapon* Weapon);
	void Disarm();
	void Arm();
	void PlayEquipMontage(const FName& SectionName);
	bool HasEnoughStamina();
	bool IsOccupied();
	bool CanDisarm();
	bool CanArm();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

private:
	
	// UI
	UPROPERTY()
	USlashOverlay* SlashOverlay;

	void InitializeSlashOverlay();
	void SetHUDHealth();

	// Equip
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	// State
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	// Components
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;

public:
	
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };
	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; };
};
