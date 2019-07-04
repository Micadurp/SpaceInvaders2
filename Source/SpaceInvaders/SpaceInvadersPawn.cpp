// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SpaceInvadersPawn.h"
#include "SpaceInvadersProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

const FName ASpaceInvadersPawn::MoveRightBinding("MoveRight");
const FName ASpaceInvadersPawn::ShootBinding("Shoot");


ASpaceInvadersPawn::ASpaceInvadersPawn()
{	

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/3D_Icons/Ship.Ship"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName("PlayerShip");
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Movement
	MoveSpeed = 800.0f;
	// Weapon
	GunOffset = FVector(50.f, 0.f, 0.f);
	FireRate = 1.0f;
	bCanFire = true;
}

// Called when the game starts or when spawned
void ASpaceInvadersPawn::BeginPlay()
{
	Super::BeginPlay();
	this->OnDestroyed.AddDynamic(this, &ASpaceInvadersPawn::OnDeath);
}

void ASpaceInvadersPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAction(ShootBinding, IE_Pressed, this, &ASpaceInvadersPawn::FireShot);
}

void ASpaceInvadersPawn::Tick(float DeltaSeconds)
{
	// Find movement direction
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	const FVector MoveDirection = FVector(0.f, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		const FRotator NewRotation = RootComponent->GetComponentRotation();
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);
		
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}
}

void ASpaceInvadersPawn::FireShot()
{
	// If it's ok to fire again
	if (bCanFire == true)
	{
		const FRotator FireRotation = FVector(1.0f, 0.f, 0.f).Rotation();
		// Spawn projectile at an offset from this pawn
		const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			// spawn the projectile
			World->SpawnActor<ASpaceInvadersProjectile>(SpawnLocation, FireRotation);
		}

		bCanFire = false;
		World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ASpaceInvadersPawn::ShotTimerExpired, FireRate); // TODO cool! timer can be used in invadercontroller?

		// try and play the sound if specified
		if (FireSound != nullptr)	
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		bCanFire = false;
	}
}

void ASpaceInvadersPawn::ShotTimerExpired()
{
	bCanFire = true;
}

void ASpaceInvadersPawn::OnDeath(AActor* DeadActor)
{
	UE_LOG(LogTemp, Log, TEXT("Game Over"))
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}
