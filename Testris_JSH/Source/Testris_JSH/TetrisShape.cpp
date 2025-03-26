// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrisShape.h"

FTetrisShape::FTetrisShape()
{
	SetRandomShape();
}

void FTetrisShape::SetRandomShape()
{
    int32 Index = FMath::RandRange(0, 6);
    ShapeType = static_cast<EShapeType>(Index);
    Points = GetShapeData(ShapeType);
    Rotation = 0;
}

bool FTetrisShape::Rotate()
{
    if (Points.Num() <= 1) return false;
    Rotation = (Rotation + 1) % Points.Num();
    return true;
}

void FTetrisShape::ResetRotation()
{
    Rotation = 0;
}

const TArray<FIntPoint>& FTetrisShape::GetBlocks() const
{
    return Points[Rotation];
}

const TArray<FIntPoint>& FTetrisShape::GetNextRotationBlocks() const
{
    return Points[(Rotation + 1) % Points.Num()];
}

EShapeType FTetrisShape::GetType() const
{
    return ShapeType;
}

TArray<TArray<FIntPoint>> FTetrisShape::GetShapeData(EShapeType Type)
{
    using T = TArray<FIntPoint>;
    switch (Type)
    {
    case EShapeType::O: 
        return 
        {
            { {0,0}, {1,0}, {0,-1}, {1,-1} } 
        };
    case EShapeType::I:
        return {
            { {0,0}, {1,0}, {-1,0}, {-2,0} }, { {0,0}, {0,1}, {0,-1}, {0,-2} } 
        };
    case EShapeType::S:
        return
        {
            { {0,0}, {1,0}, {0,-1}, {-1,-1} }, { {0,0}, {0,-1}, {1,0}, {1,1} }
        };
    case EShapeType::Z: return { { {0,0}, {-1,0}, {0,-1}, {1,-1} }, { {0,0}, {0,-1}, {-1,0}, {-1,1} } };
    case EShapeType::L: return { { {0,0}, {-1,0}, {1,0}, {1,-1} }, { {0,0}, {0,1}, {0,-1}, {1,1} }, { {0,0}, {-1,0}, {1,0}, {-1,1} }, { {0,0}, {0,1}, {0,-1}, {-1,-1} } };
    case EShapeType::J: return { { {0,0}, {-1,0}, {1,0}, {-1,-1} }, { {0,0}, {0,1}, {0,-1}, {1,-1} }, { {0,0}, {-1,0}, {1,0}, {1,1} }, { {0,0}, {0,1}, {0,-1}, {-1,1} } };
    case EShapeType::T: return { { {0,0}, {-1,0}, {1,0}, {0,-1} }, { {0,0}, {0,1}, {0,-1}, {1,0} }, { {0,0}, {-1,0}, {1,0}, {0,1} }, { {0,0}, {0,1}, {0,-1}, {-1,0} } };
    }
    return {};
}