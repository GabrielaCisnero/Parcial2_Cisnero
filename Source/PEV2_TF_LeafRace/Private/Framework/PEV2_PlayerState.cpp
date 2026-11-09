#include "Framework/PEV2_PlayerState.h"
#include "Characters/PEV2_Character.h"
#include "Net/UnrealNetwork.h"

void APEV2_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APEV2_PlayerState, Team);
	DOREPLIFETIME(APEV2_PlayerState, LeavesDelivered);
	DOREPLIFETIME(APEV2_PlayerState, LeavesStolen);
}

void APEV2_PlayerState::SetTeam(EPEV2Team NewTeam)
{
	if (!HasAuthority()) return;
	Team = NewTeam;
	OnRep_Team();
}

void APEV2_PlayerState::OnRep_Team()
{
	NotifyPawnTeamChanged();
}

void APEV2_PlayerState::NotifyPawnTeamChanged()
{
	if (APEV2_Character* Char = Cast<APEV2_Character>(GetPawn()))
	{
		Char->RefreshTeamVisuals();
	}
}
