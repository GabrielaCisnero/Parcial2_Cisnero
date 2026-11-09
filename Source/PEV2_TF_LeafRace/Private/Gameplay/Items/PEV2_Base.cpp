#include "Gameplay/Items/PEV2_Base.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

APEV2_Base::APEV2_Base()
{
	bReplicates = false;
	PrimaryActorTick.bCanEverTick = false;

	Zone = CreateDefaultSubobject<UBoxComponent>(TEXT("Zone"));
	Zone->InitBoxExtent(FVector(300.f, 300.f, 150.f));
	Zone->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SetRootComponent(Zone);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Zone);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
