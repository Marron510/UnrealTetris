// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM()
enum class EShapeType : uint8
{
	I, J, L, O, S, T, Z
};


class TESTRIS_JSH_API FTetrisShape
{
public:
	FTetrisShape();

	void SetRandomShape();
	bool Rotate();
	void ResetRotation();

	const TArray<FIntPoint>& GetBlocks() const;
	const TArray<FIntPoint>& GetNextRotationBlocks() const;
	EShapeType GetType() const;

private:
	TArray<TArray<FIntPoint>> Points;
	int Rotation = 0;
	EShapeType ShapeType;

	static TArray<TArray<FIntPoint>> GetShapeData(EShapeType Type);
};
