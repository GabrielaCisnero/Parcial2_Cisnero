// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PEV2_TF_LeafRaceGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class APEV2_TF_LeafRaceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	APEV2_TF_LeafRaceGameMode();
};



