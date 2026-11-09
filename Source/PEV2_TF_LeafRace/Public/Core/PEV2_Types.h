#pragma once

#include "CoreMinimal.h"
#include "PEV2_Types.generated.h"

UENUM(BlueprintType)
enum class EPEV2Team : uint8
{
	None UMETA(DisplayName = "Sin equipo"),
	Red  UMETA(DisplayName = "Rojo"),
	Blue UMETA(DisplayName = "Azul")
};

UENUM(BlueprintType)
enum class EPEV2MatchState : uint8
{
	Lobby    UMETA(DisplayName = "Lobby"),
	Playing  UMETA(DisplayName = "Jugando"),
	Finished UMETA(DisplayName = "Fin")
};

UENUM(BlueprintType)
enum class EPEV2InteractAction : uint8
{
	None,
	PickUp,
	Deliver,
	Steal
};
