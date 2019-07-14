// Fill out your copyright notice in the Description page of Project Settings.


#include "InvaderShip.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "InvaderProjectile.h"

// Sets default values
AInvaderShip::AInvaderShip()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>InvaderMesh1(TEXT("/Game/3D_Icons/Alien1.Alien1"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>InvaderMesh2(TEXT("/Game/3D_Icons/Alien2.Alien2"));
	// Create the mesh component
	InvaderMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InvaderMesh1"));
	RootComponent = InvaderMeshComponent;
	InvaderMeshComponent->SetCollisionProfileName("InvaderShip");
	InvaderMeshComponent->SetStaticMesh(InvaderMesh1.Object);
	InvaderShipMeshes.Add(InvaderMesh1.Object);
	InvaderShipMeshes.Add(InvaderMesh2.Object);


	GunOffset = FVector(50.f, 0.f, 0.f);
}

// Called when the game starts or when spawned
void AInvaderShip::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInvaderShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FHitResult AInvaderShip::Move(FVector destination)
{
	FHitResult Hit(1.f);

	const FRotator SameRotation = InvaderMeshComponent->GetComponentRotation();
	InvaderMeshComponent->MoveComponent(destination, SameRotation, true, &Hit);

	this->OnMove();
	return Hit;
}

void AInvaderShip::OnMove()
{
	// Changes mesh on move! Sort of "Animation"?
	InvaderShipMeshes[ShipMeshAnimation];
	ShipMeshAnimation = !ShipMeshAnimation;
}


void AInvaderShip::FireShot()
{
	const FRotator FireRotation = FVector(-1.0f, 0.f, 0.f).Rotation();
	// Spawn projectile at an offset from this pawn
	const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		// spawn the projectile
		World->SpawnActor<AInvaderProjectile>(SpawnLocation, FireRotation);
	}
}