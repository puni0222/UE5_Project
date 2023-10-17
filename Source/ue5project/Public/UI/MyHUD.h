// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

struct FInteractableData;
class UInteractionWidget;
class UMainMenu;
class USlashOverlay;

UCLASS()
class UE5PROJECT_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:

	AMyHUD();
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<USlashOverlay> SlashOverlayClass;



	bool bIsMenuVisible;
	
	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

	void ShowInteractionWidget();
	void HideInteractionWidget();
	void UpdateInteractionWidget(const FInteractableData* InteractableData);

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY()
	UMainMenu* MainMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	UPROPERTY()
	USlashOverlay* SlashOverlay;

public:

	FORCEINLINE USlashOverlay* GetSlashOverlay() const { return SlashOverlay; }
};
