// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InvaderShip.generated.h"

UCLASS(Blueprintable)
class AInvaderShip : public APawn
{
	GENERATED_BODY()

	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* InvaderMeshComponent;

	UStaticMesh* InvaderShipMesh1;
	UStaticMesh* InvaderShipMesh2;

public:
	// Sets default values for this character's properties
	AInvaderShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnMove();

	// Fire a shot
	void FireShot();

	// Returns InvaderMeshComponent subobject
	FORCEINLINE class UStaticMeshComponent* GetInvaderMeshComponent() const { return InvaderMeshComponent; }

	/** Offset from the ships location to spawn projectiles */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	FVector GunOffset;

private:
	bool ShipMeshChange = true;

};
