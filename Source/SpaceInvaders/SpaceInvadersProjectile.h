// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "SpaceInvadersProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS(config=Game)
class ASpaceInvadersProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	ASpaceInvadersProjectile();
};

