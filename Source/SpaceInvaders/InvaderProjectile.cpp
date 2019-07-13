// Fill out your copyright notice in the Description page of Project Settings.


#include "InvaderProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/StaticMesh.h"
#include "SpaceInvaders/SpaceInvadersPawn.h"

// Sets default values
AInvaderProjectile::AInvaderProjectile()
{
	// Static reference to the mesh to use for the projectile
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/3D_Icons/Bomb.Bomb"));

	ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	ProjectileMesh->BodyInstance.SetCollisionProfileName("EnemyProjectile");
}

