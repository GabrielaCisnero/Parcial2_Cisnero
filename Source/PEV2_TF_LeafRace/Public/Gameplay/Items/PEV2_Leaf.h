#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PEV2_Leaf.generated.h"

class USphereComponent;
class UStaticMeshComponent;

/** Hoja recolectable. Se oculta al ser recogida y reaparece pasado RespawnTime. */
UCLASS()
class PEV2_TF_LEAFRACE_API APEV2_Leaf : public AActor
{
	GENERATED_BODY()

public:
	APEV2_Leaf();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Leaf")
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Leaf")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leaf")
	float RespawnTime = 8.f;

	UFUNCTION(BlueprintPure, Category = "Leaf")
	bool IsAvailable() const { return bAvailable; }

	/** Solo servidor. */
	void Collect();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_Available)
	bool bAvailable = true;

	UFUNCTION()
	void OnRep_Available();

	void ApplyAvailability();
	void Respawn();

	FTimerHandle RespawnTimer;
};
