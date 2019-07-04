// Fill out your copyright notice in the Description page of Project Settings.


#include "InvaderController.h"
#include "InvaderShip.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "SpaceInvaders/SpaceInvadersPawn.h"
#include "Components/StaticMeshComponent.h"


AInvaderController::AInvaderController()
{
	PrimaryActorTick.bCanEverTick = true;	
}

// Called when the game starts or when spawned
void AInvaderController::BeginPlay() 
{
	Super::BeginPlay();
	
	NextMoveTime = BaseMoveTime;

	// TODO is there a better way to fetch all invaders? probably
	for (TActorIterator<AInvaderShip> InvaderItr(GetWorld()); InvaderItr; ++InvaderItr)
	{
		UE_LOG(LogTemp, Log, TEXT("%s reporting for duty!"), *InvaderItr->GetName())
		InitialInvaderCount++;
		InvaderItr->OnDestroyed.AddDynamic(this, &AInvaderController::OnDeath);
		InvaderItr->GetInvaderMeshComponent()->OnComponentHit.AddDynamic(this, &AInvaderController::OnHit);
	}
	
	InvaderCount = InitialInvaderCount;
	UE_LOG(LogTemp, Log, TEXT("%d invaders!"), InvaderCount)
}

// Called every frame
void AInvaderController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Move in steps rather than constantly
	if(LastMoveTime > NextMoveTime)
	{ 
		LastMoveTime = 0;
		if (!MoveDown) // Move to sides
		{
			bool DirectionChange = false;
			// Getting all AInvaderShips from world
			for (TActorIterator<AInvaderShip> InvaderItr(GetWorld()); InvaderItr; ++InvaderItr)
			{
				// TODO this feels like it can be done better
				auto InvaderMeshComp = InvaderItr->GetInvaderMeshComponent();
				const FRotator NewRotation = InvaderMeshComp->GetComponentRotation();
				FHitResult Hit(1.f);

				InvaderItr->OnMove();
				if (Direction) // Right
				{
					InvaderMeshComp->MoveComponent(FVector(0, SidewaysMovespeed, 0), NewRotation, true, &Hit);
				}
				else // Left
				{
					InvaderMeshComp->MoveComponent(FVector(0, -SidewaysMovespeed, 0), NewRotation, true, &Hit);
				}
				// Move down and direction change if at edge
				if (FMath::Abs((*InvaderItr)->GetActorLocation().Y) >= Edge)
				{
					DirectionChange = true;
					MoveDown = true;
				}

			}

			// Go reverse direction next move
			if (DirectionChange)
			{
				Direction = !Direction;
				DirectionChange = false;
			}
		}
		else // Move down
		{
			// Getting all AInvaderShips from world
			for (TActorIterator<AInvaderShip> InvaderItr(GetWorld()); InvaderItr; ++InvaderItr)
			{
				// TODO this feels like it can be done better
				auto InvaderMeshComp = InvaderItr->GetInvaderMeshComponent();
				const FRotator NewRotation = InvaderMeshComp->GetComponentRotation();
				FHitResult Hit(1.f);

				InvaderItr->OnMove();
				InvaderMeshComp->MoveComponent(FVector(-DownwardsMovespeed, 0, 0), NewRotation, true, &Hit);
			}
			MoveDown = false;
		}
	}
	else
	{
		LastMoveTime += DeltaTime;;
	}
}

// Called on death
void AInvaderController::OnDeath(AActor* deadActor)
{
	--InvaderCount;
	// Enemies start moving faster
	NextMoveTime = BaseMoveTime * ((float)InvaderCount / (float)InitialInvaderCount);

    // Check if any enemies remain
    if (InvaderCount <= 0) {
		UE_LOG(LogTemp, Log, TEXT("Victory!"))
		GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
        // TODO End Game in a better way?
	}
}

void AInvaderController::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("Hit!"))
	// KILL
	if ((OtherActor != NULL) && (OtherActor->IsA(ASpaceInvadersPawn::StaticClass())))
	{
		OtherActor->Destroy();
	}
}