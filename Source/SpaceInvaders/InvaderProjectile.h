// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "InvaderProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS()
class SPACEINVADERS_API AInvaderProjectile : public AProjectile
{
	GENERATED_BODY()


public:
	AInvaderProjectile();

};
