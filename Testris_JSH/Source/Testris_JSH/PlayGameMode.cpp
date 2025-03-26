// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayGameMode.h"
#include "TetrisController.h"


APlayGameMode::APlayGameMode()
{
	PlayerControllerClass = ATetrisController::StaticClass();
}
