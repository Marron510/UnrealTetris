// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"
#include "TetrisShape.h"
#include "Tetrimo.h"
#include "MainBoard.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TESTRIS_JSH_API AMainBoard : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainBoard();

protected:

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	void MoveLeft();
	void MoveRight();
	void MoveDown();
	void Rotate();

	bool CanMove(const FIntPoint& NewPosition, const TArray<FIntPoint>& Blocks) const;
	void LockShape();
	void SpawnNewShape();
	void SpawnWall();
	void SpawnWallBlockAt(FIntPoint GridPos);
	void UpdateVisualBlocks();
	void ClearFullLines();
	UMaterialInterface* GetMaterialForShape(EShapeType Type);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material_I;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material_O;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material_T;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material_S;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material_Z;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material_L;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material_J;


private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> WallBlockClass;

	UPROPERTY(EditAnywhere)
	float CellSize = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Board")
	int Width = 10;

	UPROPERTY(EditAnywhere, Category = "Board")
	int Height = 20;

	UPROPERTY()
	TArray<AActor*> SpawnedBlocks; 

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATetrimo> BlockClass;

	TArray<AActor*> StaticBlocks;  // 고정된 블록
	TArray<AActor*> ActiveBlocks;  // 현재 움직이는 블록

	TArray<TArray<bool>> Grid;

	FTetrisShape CurrentShape;
	FIntPoint CurrentPosition;

	float FallTimer = 0.0f;
	float FallInterval = 0.5f; // 1초마다 하강


	float WorldLocationX = -1500.0f;

	
};
