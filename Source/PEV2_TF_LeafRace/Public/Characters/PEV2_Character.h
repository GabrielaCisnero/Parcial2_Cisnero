#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/PEV2_Types.h"
#include "PEV2_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USphereComponent;
class UStaticMeshComponent;
class UInputAction;
class UInputMappingContext;
class UNiagaraSystem;
class USoundBase;
struct FInputActionValue;

UCLASS()
class PEV2_TF_LEAFRACE_API APEV2_Character : public ACharacter
{
	GENERATED_BODY()

public:
	APEV2_Character();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PawnClientRestart() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// Componentes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	/** Detecta hojas y bases cercanas. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<USphereComponent> InteractSphere;

	/** Hoja visible sobre la cabeza cuando el jugador la lleva. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UStaticMeshComponent> LeafVisual;

	//Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;

	//Efectos
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	TObjectPtr<UNiagaraSystem> InteractFX;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	TObjectPtr<USoundBase> InteractSound;

	// Estado replicado
	/** REPNOTIFY: true si lleva una hoja. Muestra/oculta la hoja sobre la cabeza en todos los jugadores. */
	UPROPERTY(ReplicatedUsing = OnRep_HasLeaf, BlueprintReadOnly, Category = "Leaf Race")
	bool bHasLeaf = false;

	// API para HUD / BP 
	/** Accion disponible ahora mismo (recoger / entregar / robar) segun lo que tenga cerca. */
	UFUNCTION(BlueprintPure, Category = "Leaf Race")
	EPEV2InteractAction GetCurrentAction(AActor*& OutTarget) const;

	/** Texto de ayuda para el HUD, ej. "[E] Recoger hoja". Vacio si no hay accion. */
	UFUNCTION(BlueprintPure, Category = "Leaf Race")
	FText GetInteractionHint() const;

	/** Llamar para (re)aplicar color de equipo. Dispara el evento BP_OnTeamChanged. */
	void RefreshTeamVisuals();

	UFUNCTION(BlueprintImplementableEvent, Category = "Leaf Race")
	void BP_OnTeamChanged(EPEV2Team NewTeam);

	// RPCs 
	/** SERVER RPC: el cliente pide interactuar; el servidor valida y ejecuta. */
	UFUNCTION(Server, Reliable)
	void Server_Interact();

	/** NETMULTICAST RPC: efecto visual/sonoro para todos los jugadores. */
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayInteractEffect(FVector Location);

protected:
	UFUNCTION()
	void OnRep_HasLeaf();

	void UpdateLeafVisual();
	void SetHasLeaf(bool bNewValue); // solo servidor

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void OnInteractPressed();
};
