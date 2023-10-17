// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyHUD.h"
#include "UI/MainMenu.h"
#include "UI/Interaction/InteractionWidget.h"
#include "HUD/SlashOverlay.h"

AMyHUD::AMyHUD()
{

}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller)
		{
			if (SlashOverlayClass)
			{
				SlashOverlay = CreateWidget<USlashOverlay>(Controller, SlashOverlayClass);
				SlashOverlay->AddToViewport();
			}

			if (MainMenuClass)
			{
				MainMenuWidget = CreateWidget<UMainMenu>(Controller, MainMenuClass);
				MainMenuWidget->AddToViewport();
				MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
			}

			if (InteractionWidgetClass)
			{
				InteractionWidget = CreateWidget<UInteractionWidget>(Controller, InteractionWidgetClass);
				InteractionWidget->AddToViewport();
				InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void AMyHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMyHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMyHUD::ToggleMenu()
{
	if (bIsMenuVisible)
	{
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();

		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void AMyHUD::ShowInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMyHUD::HideInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMyHUD::UpdateInteractionWidget(const FInteractableData* InteractableData)
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}


