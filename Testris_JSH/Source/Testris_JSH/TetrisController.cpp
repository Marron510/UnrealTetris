// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrisController.h"
#include "EngineUtils.h"
#include "MainBoard.h"

ATetrisController::ATetrisController()
{
    InputComponent = CreateDefaultSubobject<UInputComponent>(TEXT("InputComponent"));
}

void ATetrisController::BeginPlay()
{
    Super::BeginPlay();
    Board = Cast<AMainBoard>(GetPawn());
}

void ATetrisController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("Left", IE_Pressed, this, &ATetrisController::OnMoveLeft);
    InputComponent->BindAction("Right", IE_Pressed, this, &ATetrisController::OnMoveRight);
    InputComponent->BindAction("Down", IE_Pressed, this, &ATetrisController::OnMoveDown);
    InputComponent->BindAction("Rotate", IE_Pressed, this, &ATetrisController::OnRotate);
}

void ATetrisController::OnMoveLeft()
{
    if (Board)
        Board->MoveLeft();
    Board->UpdateVisualBlocks();
}

void ATetrisController::OnMoveRight()
{
    if (Board) 
        Board->MoveRight(); 
    Board->UpdateVisualBlocks();
}

void ATetrisController::OnMoveDown() 
{
    if (Board) 
        Board->MoveDown();
    Board->UpdateVisualBlocks();
}

void ATetrisController::OnRotate()
{ 
    if (Board)
        Board->Rotate(); 
    Board->UpdateVisualBlocks();
}