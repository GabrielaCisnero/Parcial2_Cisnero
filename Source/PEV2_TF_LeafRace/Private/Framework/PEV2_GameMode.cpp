#include "Framework/PEV2_GameMode.h"
#include "Framework/PEV2_GameState.h"
#include "Framework/PEV2_PlayerState.h"
#include "Framework/PEV2_PlayerController.h"
#include "Characters/PEV2_Character.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "TimerManager.h"
#include "Engine/World.h"

APEV2_GameMode::APEV2_GameMode()
{
	GameStateClass = APEV2_GameState::StaticClass();
	PlayerStateClass = APEV2_PlayerState::StaticClass();
	PlayerControllerClass = APEV2_PlayerController::StaticClass();
	DefaultPawnClass = APEV2_Character::StaticClass();
}

void APEV2_GameMode::BeginPlay()
{
	Super::BeginPlay();

	if (APEV2_GameState* GS = GetGameState<APEV2_GameState>())
	{
		GS->TargetLeaves = TargetLeaves;
		GS->RemainingTime = WarmupTime;
		GS->SetMatchState(EPEV2MatchState::Lobby);
	}

	GetWorldTimerManager().SetTimer(MatchTimer, this, &APEV2_GameMode::MatchTick, 1.f, true);
}

void APEV2_GameMode::PostLogin(APlayerController* NewPlayer)
{
	// Asignamos el equipo ANTES del Super: el Super spawnea el pawn y necesita saber el equipo.
	if (APEV2_PlayerState* PS = NewPlayer ? NewPlayer->GetPlayerState<APEV2_PlayerState>() : nullptr)
	{
		int32 Red = 0, Blue = 0;
		if (GameState)
		{
			for (APlayerState* Other : GameState->PlayerArray)
			{
				if (const APEV2_PlayerState* OtherPS = Cast<APEV2_PlayerState>(Other))
				{
					if (OtherPS->Team == EPEV2Team::Red) ++Red;
					else if (OtherPS->Team == EPEV2Team::Blue) ++Blue;
				}
			}
		}
		PS->SetTeam(Red <= Blue ? EPEV2Team::Red : EPEV2Team::Blue);
	}

	Super::PostLogin(NewPlayer);
}

AActor* APEV2_GameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	const APEV2_PlayerState* PS = Player ? Player->GetPlayerState<APEV2_PlayerState>() : nullptr;
	if (PS && PS->Team != EPEV2Team::None)
	{
		// En el editor: seleccionar cada PlayerStart y ponerle Player Start Tag = "Red" o "Blue".
		const FName Tag = (PS->Team == EPEV2Team::Red) ? FName("Red") : FName("Blue");
		TArray<APlayerStart*> Starts;
		for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
		{
			if (It->PlayerStartTag == Tag) Starts.Add(*It);
		}
		if (Starts.Num() > 0)
		{
			return Starts[FMath::RandRange(0, Starts.Num() - 1)];
		}
	}
	return Super::ChoosePlayerStart_Implementation(Player);
}

void APEV2_GameMode::MatchTick()
{
	APEV2_GameState* GS = GetGameState<APEV2_GameState>();
	if (!GS) return;

	if (GS->CurrentMatchState == EPEV2MatchState::Lobby)
	{
		GS->RemainingTime = FMath::Max(0, GS->RemainingTime - 1);
		if (GS->RemainingTime <= 0) StartMatch();
	}
	else if (GS->CurrentMatchState == EPEV2MatchState::Playing)
	{
		GS->RemainingTime = FMath::Max(0, GS->RemainingTime - 1);
		if (GS->RemainingTime <= 0)
		{
			EPEV2Team Winner = EPEV2Team::None; // empate
			if (GS->RedLeaves > GS->BlueLeaves) Winner = EPEV2Team::Red;
			else if (GS->BlueLeaves > GS->RedLeaves) Winner = EPEV2Team::Blue;
			EndMatch(Winner);
		}
	}
}

void APEV2_GameMode::StartMatch()
{
	if (APEV2_GameState* GS = GetGameState<APEV2_GameState>())
	{
		GS->RemainingTime = MatchDuration;
		GS->SetMatchState(EPEV2MatchState::Playing);
	}
}

void APEV2_GameMode::EndMatch(EPEV2Team Winner)
{
	APEV2_GameState* GS = GetGameState<APEV2_GameState>();
	if (!GS || GS->CurrentMatchState != EPEV2MatchState::Playing) return;

	GetWorldTimerManager().ClearTimer(MatchTimer);
	GS->SetMatchState(EPEV2MatchState::Finished, Winner);

	// Client RPC a cada jugador con la pantalla final.
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APEV2_PlayerController* PC = Cast<APEV2_PlayerController>(It->Get()))
		{
			PC->Client_ShowEndScreen(Winner);
		}
	}
}

void APEV2_GameMode::AddLeafToTeam(EPEV2Team Team)
{
	APEV2_GameState* GS = GetGameState<APEV2_GameState>();
	if (!GS || GS->CurrentMatchState != EPEV2MatchState::Playing) return;

	GS->ModifyLeaves(Team, +1);
	if (GS->GetLeaves(Team) >= TargetLeaves)
	{
		EndMatch(Team);
	}
}

bool APEV2_GameMode::TryStealLeaf(EPEV2Team VictimTeam)
{
	APEV2_GameState* GS = GetGameState<APEV2_GameState>();
	if (!GS || GS->CurrentMatchState != EPEV2MatchState::Playing) return false;
	if (GS->GetLeaves(VictimTeam) <= 0) return false;

	GS->ModifyLeaves(VictimTeam, -1);
	return true;
}
