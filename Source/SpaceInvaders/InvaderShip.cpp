// Fill out your copyright notice in the Description page of Project Settings.


#include "InvaderShip.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AInvaderShip::AInvaderShip()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>InvaderMesh1(TEXT("/Game/3D_Icons/Alien1.Alien1"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>InvaderMesh2(TEXT("/Game/3D_Icons/Alien2.Alien2"));
	// Create the mesh component
	InvaderMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InvaderMesh1"));
	RootComponent = InvaderMeshComponent;
	//ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	InvaderMeshComponent->SetStaticMesh(InvaderMesh1.Object);
	InvaderShipMesh1 = InvaderMesh1.Object;
	InvaderShipMesh2 = InvaderMesh2.Object;
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

// Changes mesh on move!
void AInvaderShip::OnMove()
{
	if (ShipMeshChange)
	{
		InvaderMeshComponent->SetStaticMesh(InvaderShipMesh2);
		ShipMeshChange = false;
	}
	else
	{
		InvaderMeshComponent->SetStaticMesh(InvaderShipMesh1);
		ShipMeshChange = true;
	}
}

