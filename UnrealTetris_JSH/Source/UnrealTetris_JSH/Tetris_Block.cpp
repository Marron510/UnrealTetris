// Fill out your copyright notice in the Description page of Project Settings.


#include "Tetris_Block.h"

// Sets default values
ATetris_Block::ATetris_Block()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATetris_Block::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATetris_Block::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATetris_Block::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

