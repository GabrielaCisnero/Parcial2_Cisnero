#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/PEV2_Types.h"
#include "PEV2_GameMode.generated.h"

UCLASS()
class PEV2_TF_LEAFRACE_API APEV2_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APEV2_GameMode();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override { return false; }
	
	/** Duracion de la partida. */
	UPROPERTY(EditDefaultsOnly, Category = "Leaf Race")
	int32 MatchDuration = 300;

	/** Cuenta regresiva previa al inicio. */
	UPROPERTY(EditDefaultsOnly, Category = "Leaf Race")
	int32 WarmupTime = 10;

	/** Hojas en la base para ganar de forma anticipada. */
	UPROPERTY(EditDefaultsOnly, Category = "Leaf Race")
	int32 TargetLeaves = 15;

	//Llamados por los personajes (solo servidor)
	void AddLeafToTeam(EPEV2Team Team);
	bool TryStealLeaf(EPEV2Team VictimTeam);

protected:
	void MatchTick();
	void StartMatch();
	void EndMatch(EPEV2Team Winner);

	FTimerHandle MatchTimer;
};
