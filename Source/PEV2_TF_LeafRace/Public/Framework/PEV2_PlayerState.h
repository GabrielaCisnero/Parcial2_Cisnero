#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Core/PEV2_Types.h"
#include "PEV2_PlayerState.generated.h"

UCLASS()
class PEV2_TF_LEAFRACE_API APEV2_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Equipo asignado. RepNotify: actualiza el color/visual del personaje en los clientes. */
	UPROPERTY(ReplicatedUsing = OnRep_Team, BlueprintReadOnly, Category = "Leaf Race")
	EPEV2Team Team = EPEV2Team::None;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Leaf Race")
	int32 LeavesDelivered = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Leaf Race")
	int32 LeavesStolen = 0;

	/** Solo servidor. */
	void SetTeam(EPEV2Team NewTeam);

protected:
	UFUNCTION()
	void OnRep_Team();

	void NotifyPawnTeamChanged();
};
