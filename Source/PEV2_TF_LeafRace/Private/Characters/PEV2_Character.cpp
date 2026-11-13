#include "Characters/PEV2_Character.h"
#include "Framework/PEV2_GameMode.h"
#include "Framework/PEV2_GameState.h"
#include "Framework/PEV2_PlayerState.h"
#include "Framework/PEV2_PlayerController.h"
#include "Gameplay/Items/PEV2_Leaf.h"
#include "Gameplay/Items/PEV2_Base.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"

APEV2_Character::APEV2_Character()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -96.f), FRotator(0.f, -90.f, 0.f));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractSphere"));
	InteractSphere->SetupAttachment(RootComponent);
	InteractSphere->InitSphereRadius(180.f);
	InteractSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	LeafVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeafVisual"));
	LeafVisual->SetupAttachment(GetCapsuleComponent());
	LeafVisual->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	LeafVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeafVisual->SetVisibility(false);
}

void APEV2_Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APEV2_Character, bHasLeaf);
}

// Input
void APEV2_Character::PawnClientRestart()
{
	Super::PawnClientRestart();

	if (const APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Sub =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Sub->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void APEV2_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)     EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APEV2_Character::Move);
		if (LookAction)     EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &APEV2_Character::Look);
		if (JumpAction)
		{
			EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}
		if (InteractAction) EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &APEV2_Character::OnInteractPressed);
	}
}

void APEV2_Character::Move(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	if (!Controller) return;

	const FRotator YawRot(0.f, Controller->GetControlRotation().Yaw, 0.f);
	AddMovementInput(FRotationMatrix(YawRot).GetUnitAxis(EAxis::X), Axis.Y);
	AddMovementInput(FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y), Axis.X);
}

void APEV2_Character::Look(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void APEV2_Character::OnInteractPressed()
{
	// El cliente solo pide; el servidor decide.
	Server_Interact();
}

// Equipo
void APEV2_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	RefreshTeamVisuals(); // servidor
}

void APEV2_Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	RefreshTeamVisuals(); // cliente
}

void APEV2_Character::RefreshTeamVisuals()
{
	if (const APEV2_PlayerState* PS = GetPlayerState<APEV2_PlayerState>())
	{
		if (PS->Team != EPEV2Team::None)
		{
			BP_OnTeamChanged(PS->Team);
		}
	}
}

// Hoja (RepNotify)
void APEV2_Character::OnRep_HasLeaf()
{
	UpdateLeafVisual();
}

void APEV2_Character::UpdateLeafVisual()
{
	LeafVisual->SetVisibility(bHasLeaf);
}

void APEV2_Character::SetHasLeaf(bool bNewValue)
{
	if (!HasAuthority()) return;
	bHasLeaf = bNewValue;
	OnRep_HasLeaf(); // el host tambien debe actualizar su visual
}

// Interaccion
EPEV2InteractAction APEV2_Character::GetCurrentAction(AActor*& OutTarget) const
{
	OutTarget = nullptr;

	const APEV2_GameState* GS = GetWorld() ? GetWorld()->GetGameState<APEV2_GameState>() : nullptr;
	const APEV2_PlayerState* PS = GetPlayerState<APEV2_PlayerState>();
	if (!GS || !PS || PS->Team == EPEV2Team::None) return EPEV2InteractAction::None;
	if (GS->CurrentMatchState != EPEV2MatchState::Playing) return EPEV2InteractAction::None;

	TArray<AActor*> Overlaps;
	InteractSphere->GetOverlappingActors(Overlaps);

	AActor* LeafTarget = nullptr;
	for (AActor* A : Overlaps)
	{
		if (const APEV2_Base* Base = Cast<APEV2_Base>(A))
		{
			if (bHasLeaf && Base->Team == PS->Team)
			{
				OutTarget = const_cast<APEV2_Base*>(Base);
				return EPEV2InteractAction::Deliver;
			}
			if (!bHasLeaf && Base->Team != PS->Team && GS->GetLeaves(Base->Team) > 0)
			{
				OutTarget = const_cast<APEV2_Base*>(Base);
				return EPEV2InteractAction::Steal;
			}
		}
		else if (!bHasLeaf && !LeafTarget)
		{
			if (APEV2_Leaf* Leaf = Cast<APEV2_Leaf>(A))
			{
				if (Leaf->IsAvailable()) LeafTarget = Leaf;
			}
		}
	}

	if (LeafTarget)
	{
		OutTarget = LeafTarget;
		return EPEV2InteractAction::PickUp;
	}
	return EPEV2InteractAction::None;
}

FText APEV2_Character::GetInteractionHint() const
{
	AActor* Target = nullptr;
	switch (GetCurrentAction(Target))
	{
	case EPEV2InteractAction::PickUp:  return FText::FromString(TEXT("[E] Recoger hoja"));
	case EPEV2InteractAction::Deliver: return FText::FromString(TEXT("[E] Entregar hoja"));
	case EPEV2InteractAction::Steal:   return FText::FromString(TEXT("[E] Robar hoja"));
	default:                           return FText::GetEmpty();
	}
}

void APEV2_Character::Server_Interact_Implementation()
{
	// Se ejecuta solo en el servidor, la validacion se hace aca.
	APEV2_GameMode* GM = GetWorld()->GetAuthGameMode<APEV2_GameMode>();
	APEV2_PlayerState* PS = GetPlayerState<APEV2_PlayerState>();
	APEV2_PlayerController* PC = Cast<APEV2_PlayerController>(GetController());
	if (!GM || !PS) return;

	AActor* Target = nullptr;
	const EPEV2InteractAction Action = GetCurrentAction(Target);
	if (!Target) return;

	switch (Action)
	{
	case EPEV2InteractAction::PickUp:
		if (APEV2_Leaf* Leaf = Cast<APEV2_Leaf>(Target))
		{
			Leaf->Collect();
			SetHasLeaf(true);
			Multicast_PlayInteractEffect(Leaf->GetActorLocation());
			if (PC) PC->Client_ShowFeedback(TEXT("Hoja recogida! Llevala a tu base."));
		}
		break;

	case EPEV2InteractAction::Deliver:
		GM->AddLeafToTeam(PS->Team);
		PS->LeavesDelivered++;
		SetHasLeaf(false);
		Multicast_PlayInteractEffect(Target->GetActorLocation());
		if (PC) PC->Client_ShowFeedback(TEXT("Hoja entregada! +1 para tu equipo."));
		break;

	case EPEV2InteractAction::Steal:
		if (const APEV2_Base* Base = Cast<APEV2_Base>(Target))
		{
			if (GM->TryStealLeaf(Base->Team))
			{
				PS->LeavesStolen++;
				SetHasLeaf(true);
				Multicast_PlayInteractEffect(Target->GetActorLocation());
				if (PC) PC->Client_ShowFeedback(TEXT("Hoja robada! Corre a tu base."));
			}
		}
		break;

	default:
		break;
	}
}

void APEV2_Character::Multicast_PlayInteractEffect_Implementation(FVector Location)
{
	if (InteractFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), InteractFX, Location);
	}
	if (InteractSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, InteractSound, Location);
	}
}
