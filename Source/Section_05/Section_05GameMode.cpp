// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Section_05GameMode.h"
#include "Section_05HUD.h"
#include "Player/FirstPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASection_05GameMode::ASection_05GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Player/Behaviour/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASection_05HUD::StaticClass();
}
