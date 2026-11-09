#include "Framework/PEV2_GameState.h"
#include "Net/UnrealNetwork.h"

APEV2_GameState::APEV2_GameState()
{
}

void APEV2_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APEV2_GameState, CurrentMatchState);
	DOREPLIFETIME(APEV2_GameState, RemainingTime);
	DOREPLIFETIME(APEV2_GameState, RedLeaves);
	DOREPLIFETIME(APEV2_GameState, BlueLeaves);
	DOREPLIFETIME(APEV2_GameState, TargetLeaves);
	DOREPLIFETIME(APEV2_GameState, WinningTeam);
}

int32 APEV2_GameState::GetLeaves(EPEV2Team Team) const
{
	switch (Team)
	{
	case EPEV2Team::Red:  return RedLeaves;
	case EPEV2Team::Blue: return BlueLeaves;
	default:              return 0;
	}
}

void APEV2_GameState::ModifyLeaves(EPEV2Team Team, int32 Delta)
{
	if (!HasAuthority()) return;

	if (Team == EPEV2Team::Red)       RedLeaves = FMath::Max(0, RedLeaves + Delta);
	else if (Team == EPEV2Team::Blue) BlueLeaves = FMath::Max(0, BlueLeaves + Delta);

	OnRep_Scores(); // el host/servidor no recibe OnRep, lo llamamos a mano
}

void APEV2_GameState::SetMatchState(EPEV2MatchState NewState, EPEV2Team Winner)
{
	if (!HasAuthority()) return;

	WinningTeam = Winner;
	CurrentMatchState = NewState;
	OnRep_MatchState();
}

void APEV2_GameState::OnRep_MatchState()
{
	OnMatchStateChanged.Broadcast();
}

void APEV2_GameState::OnRep_Scores()
{
	OnScoresChanged.Broadcast();
}
