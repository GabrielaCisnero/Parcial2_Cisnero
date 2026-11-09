#include "Gameplay/Items/PEV2_Leaf.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

APEV2_Leaf::APEV2_Leaf()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->InitSphereRadius(60.f);
	Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SetRootComponent(Sphere);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Sphere);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APEV2_Leaf::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APEV2_Leaf, bAvailable);
}

void APEV2_Leaf::Collect()
{
	if (!HasAuthority() || !bAvailable) return;

	bAvailable = false;
	ApplyAvailability();
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &APEV2_Leaf::Respawn, RespawnTime, false);
}

void APEV2_Leaf::Respawn()
{
	bAvailable = true;
	ApplyAvailability();
}

void APEV2_Leaf::OnRep_Available()
{
	ApplyAvailability();
}

void APEV2_Leaf::ApplyAvailability()
{
	SetActorHiddenInGame(!bAvailable);
	SetActorEnableCollision(bAvailable);
}
