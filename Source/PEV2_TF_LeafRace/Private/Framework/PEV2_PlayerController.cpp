#include "Framework/PEV2_PlayerController.h"
#include "Framework/PEV2_PlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"

void APEV2_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		SetInputMode(FInputModeGameOnly());
		if (HUDClass)
		{
			HUDWidget = CreateWidget<UUserWidget>(this, HUDClass);
			if (HUDWidget) HUDWidget->AddToViewport(0);
		}
	}
}

void APEV2_PlayerController::Client_ShowEndScreen_Implementation(EPEV2Team Winner)
{
	const APEV2_PlayerState* PS = GetPlayerState<APEV2_PlayerState>();
	WinnerTeam = Winner;
	bIsDraw = (Winner == EPEV2Team::None);
	bLocalTeamWon = PS && !bIsDraw && PS->Team == Winner;

	// Los datos se setean antes de crear el widget para que su Construct ya los lea.
	if (EndScreenClass && !EndScreenWidget)
	{
		EndScreenWidget = CreateWidget<UUserWidget>(this, EndScreenClass);
		if (EndScreenWidget) EndScreenWidget->AddToViewport(10);
	}

	FInputModeUIOnly Mode;
	SetInputMode(Mode);
	bShowMouseCursor = true;

	BP_OnMatchEnded(Winner, bLocalTeamWon);
}

void APEV2_PlayerController::Client_ShowFeedback_Implementation(const FString& Message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, Message);
	}
	BP_OnFeedback(Message);
}
