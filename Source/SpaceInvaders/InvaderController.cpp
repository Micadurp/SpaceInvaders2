// Fill out your copyright notice in the Description page of Project Settings.


#include "InvaderController.h"
#include "InvaderShip.h"

AInvaderController::AInvaderController()
{
	PrimaryActorTick.bCanEverTick = true;	
}

// Called when the game starts or when spawned
void AInvaderController::BeginPlay() 
{
	Super::BeginPlay();
	int InitialInvaderCount = 0;
	
	// TODO is there a better way to fetch all invaders? probably
	for (TActorIterator<AInvaderShip> InvaderItr(GetWorld()); InvaderItr; ++InvaderItr)
	{
		UE_LOG(LogTemp, Log, TEXT("%s reporting for duty!"), *InvaderItr->GetName())
		InitialInvaderCount++;
		//Invaders->OnDestroyed.AddDynamic(this, &AInvaderController::OnDeath);
	}

	InvaderCount = InitialInvaderCount;
	UE_LOG(LogTemp, Log, TEXT("%d invaders!"), InvaderCount)
}

// Called every frame
void AInvaderController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bool DirectionChange = false;
	for (TActorIterator<AInvaderShip> InvaderItr(GetWorld()); InvaderItr; ++InvaderItr)
	{
		//UE_LOG(LogTemp, Log, TEXT("%s reporting for duty!"), *InvaderItr->GetName())
		InvaderItr->OnMove();
		if (Direction) // Right
		{
			(*InvaderItr)->AddActorWorldOffset(FVector(0, 4, 0));
		}
		else // Left
		{
			(*InvaderItr)->AddActorWorldOffset(FVector(0, -4, 0));
		}
		// Move down and go opposite direction if at edge
		if (FMath::Abs((*InvaderItr)->GetActorLocation().Y) >= Edge) 
		{
			DirectionChange = true;
			for (TActorIterator<AInvaderShip> InnerInvaderItr(GetWorld()); InnerInvaderItr; ++InnerInvaderItr)
			{
				(*InnerInvaderItr)->AddActorWorldOffset(FVector(-20, 0, 0));
			}
		}
	}
	// Done outside the loop so it affects all invaders at once
	if (DirectionChange)
	{
		Direction = !Direction;
		DirectionChange = false;
	}
}

// Called on death
void AInvaderController::OnDeath() {

}