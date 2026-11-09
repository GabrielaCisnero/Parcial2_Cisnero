#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Core/PEV2_Types.h"
#include "PEV2_GameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPEV2ScoresChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPEV2MatchStateChanged);

UCLASS()
class PEV2_TF_LEAFRACE_API APEV2_GameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	APEV2_GameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Estado de la partida (Lobby / Jugando / Fin). RepNotify para que los clientes actualicen la UI. */
	UPROPERTY(ReplicatedUsing = OnRep_MatchState, BlueprintReadOnly, Category = "Leaf Race")
	EPEV2MatchState CurrentMatchState = EPEV2MatchState::Lobby;

	/** Segundos restantes (warmup en Lobby, partida en Playing). */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Leaf Race")
	int32 RemainingTime = 0;

	/** Hojas almacenadas en la base de cada equipo. */
	UPROPERTY(ReplicatedUsing = OnRep_Scores, BlueprintReadOnly, Category = "Leaf Race")
	int32 RedLeaves = 0;

	UPROPERTY(ReplicatedUsing = OnRep_Scores, BlueprintReadOnly, Category = "Leaf Race")
	int32 BlueLeaves = 0;

	/** Hojas necesarias para ganar antes de que termine el tiempo. */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Leaf Race")
	int32 TargetLeaves = 15;

	/** Equipo ganador (None = empate). Valido cuando CurrentMatchState == Finished. */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Leaf Race")
	EPEV2Team WinningTeam = EPEV2Team::None;

	UPROPERTY(BlueprintAssignable, Category = "Leaf Race")
	FOnPEV2ScoresChanged OnScoresChanged;

	UPROPERTY(BlueprintAssignable, Category = "Leaf Race")
	FOnPEV2MatchStateChanged OnMatchStateChanged;

	UFUNCTION(BlueprintPure, Category = "Leaf Race")
	int32 GetLeaves(EPEV2Team Team) const;

	// --- Solo servidor ---
	void ModifyLeaves(EPEV2Team Team, int32 Delta);
	void SetMatchState(EPEV2MatchState NewState, EPEV2Team Winner = EPEV2Team::None);

protected:
	UFUNCTION()
	void OnRep_MatchState();

	UFUNCTION()
	void OnRep_Scores();
};
