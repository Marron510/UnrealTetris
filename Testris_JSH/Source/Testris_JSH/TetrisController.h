// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainBoard.h"
#include "TetrisController.generated.h"

/**
 * 
 */
UCLASS()
class TESTRIS_JSH_API ATetrisController : public APlayerController
{
	GENERATED_BODY()

public:
	ATetrisController();
	virtual void BeginPlay() override;
protected:
	virtual void SetupInputComponent() override;
	
	UPROPERTY()
	AMainBoard* Board;

public:
	void OnMoveLeft();
	void OnMoveRight();
	void OnMoveDown();
	void OnRotate();
};
