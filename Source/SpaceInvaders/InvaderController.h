// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"
#include "InvaderController.generated.h"

UCLASS()
class SPACEINVADERS_API AInvaderController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AInvaderController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Handle death
	void OnDeath();

private:
	int InvaderCount = 0;
	// Two directions, true is right, false is left
	bool Direction = true;

	//Edge for going down
	float Edge = 800;
};
