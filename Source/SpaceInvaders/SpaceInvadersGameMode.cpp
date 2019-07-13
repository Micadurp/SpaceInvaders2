// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SpaceInvadersGameMode.h"
#include "DefenderPawn.h"

ASpaceInvadersGameMode::ASpaceInvadersGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ADefenderPawn::StaticClass();
}

