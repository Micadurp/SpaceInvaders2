// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InvaderShip.h"
#include "InvaderController.generated.h"

UCLASS()
class SPACEINVADERS_API AInvaderController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AInvaderController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Handle death
	UFUNCTION()
	void OnDeath(AActor* Actor, EEndPlayReason::Type EndPlayReason);

	// On hitting the player!
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Move big blob of invaders
	void MoveInvaderMass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	int InitialInvaderCount = 0;
	int InvaderCount = 0;

	// Two directions, true is right, false is left
	bool Direction = true;
	bool MoveDown = false;

	//Edge for going down
	float Edge = 800.0f;
	//Bottom of level
	float Bottom = -800.0f;

	// Time since last move
	float LastMoveTime = 0.0f;
	// Next time to move
	float NextMoveTime = 1.0f;

	// Size changes on death, be careful!
	TArray<AInvaderShip*> Invaders;

	
public:
	//How often enemies move
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float BaseMoveTime = 1.0f;

	//How far enemies move sideways each step
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float SidewaysMovespeed = 50;

	//How far enemies move down each step
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float DownwardsMovespeed = 50;

	// Higher the number the harder it is
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	int Difficulty = 1;
	// How difficult it gets later, Lower means harder
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	int DifficultyScaling = 10;
};
