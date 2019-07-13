// Fill out your copyright notice in the Description page of Project Settings.


#include "InvaderController.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "SpaceInvaders/DefenderPawn.h"
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

	// TODO is there a better way to fetch all invaders?
	for (TActorIterator<AInvaderShip> InvaderItr(GetWorld()); InvaderItr; ++InvaderItr)
	{
		UE_LOG(LogTemp, Log, TEXT("%s reporting for duty!"), *InvaderItr->GetName())
		InitialInvaderCount++;
		InvaderItr->OnEndPlay.AddDynamic(this, &AInvaderController::OnDeath);
		InvaderItr->GetInvaderMeshComponent()->OnComponentHit.AddDynamic(this, &AInvaderController::OnHit);
		Invaders.Add(*InvaderItr);
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
		MoveInvaderMass();
	}
	else
	{
		LastMoveTime += DeltaTime;;
	}
}

// Called on death
void AInvaderController::OnDeath(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		Invaders.RemoveSwap((AInvaderShip*)Actor);
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
}

void AInvaderController::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("Hit!"))
	// KILL
	if ((OtherActor != NULL) && (OtherActor->IsA(ADefenderPawn::StaticClass())))
	{
		OtherActor->Destroy();
	}
}

void AInvaderController::MoveInvaderMass()
{
	TArray<AInvaderShip*> tempInvadersArray = TArray<AInvaderShip*>(Invaders);
	FVector InvaderMove;

	if (!MoveDown)
	{
		float InvaderMoveSideways = (Direction ? 1 : -1) * SidewaysMovespeed;
		InvaderMove = FVector(0, InvaderMoveSideways, 0);
	}
	else
	{
		InvaderMove = FVector(-DownwardsMovespeed, 0, 0);
	}

	bool DirectionChange = false;

	for (auto Invader : tempInvadersArray)
	{
		Invader->Move(InvaderMove);

		// Move down and direction change if at edge
		if (FMath::Abs(Invader->GetActorLocation().Y) >= Edge)
		{
			DirectionChange = true;
		}

		// Enemies randomly shoot 
		if (FMath::RandRange(1, InvaderCount + DifficultyScaling) <= Difficulty)
		{
			Invader->FireShot();
		}
	}

	// Go reverse direction and also move down
	if (DirectionChange && !MoveDown)
	{
		Direction = !Direction;
		MoveDown = true;
	}
	else
	{
		MoveDown = false;
	}
}
