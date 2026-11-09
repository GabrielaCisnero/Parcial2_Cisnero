#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/PEV2_Types.h"
#include "PEV2_PlayerController.generated.h"

class UUserWidget;

UCLASS()
class PEV2_TF_LEAFRACE_API APEV2_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	/** Widget del HUD (WBP_HUD). Se asigna en BP_PlayerController. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDClass;

	/** Widget de fin de partida (WBP_EndScreen). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> EndScreenClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TObjectPtr<UUserWidget> HUDWidget;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TObjectPtr<UUserWidget> EndScreenWidget;

	/** Datos para que WBP_EndScreen muestre Victoria / Derrota / Empate. */
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	EPEV2Team WinnerTeam = EPEV2Team::None;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	bool bLocalTeamWon = false;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	bool bIsDraw = false;

	/** CLIENT RPC: el servidor avisa SOLO a este cliente del fin de partida. */
	UFUNCTION(Client, Reliable)
	void Client_ShowEndScreen(EPEV2Team Winner);

	/** CLIENT RPC: feedback puntual (hoja recogida, entregada, robada...). */
	UFUNCTION(Client, Unreliable)
	void Client_ShowFeedback(const FString& Message);

	/** Para mostrar el mensaje en un widget propio (opcional). */
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void BP_OnFeedback(const FString& Message);

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void BP_OnMatchEnded(EPEV2Team Winner, bool bWon);
};
