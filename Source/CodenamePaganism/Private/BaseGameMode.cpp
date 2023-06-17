// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"
#include "Character/MainPlayerController.h"
#include "Character/BaseCharacter.h"

ABaseGameMode::ABaseGameMode()
{
	DefaultPawnClass = ABaseCharacter::StaticClass();
	PlayerControllerClass = AMainPlayerController::StaticClass();
}
