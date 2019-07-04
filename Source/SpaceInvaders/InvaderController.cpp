// Fill out your copyright notice in the Description page of Project Settings.


#include "InvaderController.h"
#include "InvaderShip.h"
#include "EngineUtils.h"

AInvaderController::AInvaderController()
{
	PrimaryActorTick.bCanEverTick = true;	
}

// Called when the game starts or when spawned
void AInvaderController::BeginPlay() 
{
	Super::BeginPlay();
	
	// TODO is there a better way to fetch all invaders? probably
	for (TActorIterator<AInvaderShip> InvaderItr(GetWorld()); InvaderItr; ++InvaderItr)
	{
		UE_LOG(LogTemp, Log, TEXT("%s reporting for duty!"), *InvaderItr->GetName())
		InitialInvaderCount++;
		InvaderItr->OnDestroyed.AddDynamic(this, &AInvaderController::OnDeath);
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
				InvaderItr->OnMove();
				if (Direction) // Right
				{
					(*InvaderItr)->AddActorWorldOffset(FVector(0, SidewaysMovespeed, 0));
				}
				else // Left
				{
					(*InvaderItr)->AddActorWorldOffset(FVector(0, -SidewaysMovespeed, 0));
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
				InvaderItr->OnMove();
				(*InvaderItr)->AddActorWorldOffset(FVector(-DownwardsMovespeed, 0, 0));
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
        //End Game
	}
}