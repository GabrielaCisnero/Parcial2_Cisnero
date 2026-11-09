#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/PEV2_Types.h"
#include "PEV2_Base.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

/** Zona de entrega / base de un equipo. Se coloca en el nivel y se elige el equipo en Details. */
UCLASS()
class PEV2_TF_LEAFRACE_API APEV2_Base : public AActor
{
	GENERATED_BODY()

public:
	APEV2_Base();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base")
	TObjectPtr<UBoxComponent> Zone;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base")
	TObjectPtr<UStaticMeshComponent> Mesh;

	/** Equipo dueno de esta base. Es estatico (actor del nivel), no necesita replicarse. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base")
	EPEV2Team Team = EPEV2Team::Red;
};
