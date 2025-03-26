// Fill out your copyright notice in the Description page of Project Settings.


#include "MainBoard.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMainBoard::AMainBoard()
{
	PrimaryActorTick.bCanEverTick = true;
    CurrentPosition = { Width / 2, Height - 1 };

    // 카메라 
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpring"));
    CameraBoom->SetupAttachment(RootComponent);
    
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom);
    FollowCamera->bUsePawnControlRotation = false;
    FollowCamera->SetRelativeLocation(FVector{ WorldLocationX, Width * -0.5f * CellSize , Height * 0.7f * CellSize });
    FollowCamera->SetRelativeRotation(FRotator(0.f, 180.f, 0.f ));

}

// Called when the game starts or when spawned
void AMainBoard::BeginPlay()
{
	Super::BeginPlay();
    
    Grid.SetNum(Width);
    for (int x = 0; x < Width; ++x)
    {
        Grid[x].SetNum(Height);
        for (int y = 0; y < Height; ++y)
        {
            Grid[x][y] = false;
        }
    }


    SpawnWall();
    SpawnNewShape();

}

// Called every frame
void AMainBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    FallTimer += DeltaTime;
    if (FallTimer >= FallInterval)
    {
        MoveDown();
        FallTimer = 0.0f;
        UpdateVisualBlocks();
    }
}

// Called to bind functionality to input
void AMainBoard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
}


void AMainBoard::SpawnNewShape()
{
    CurrentShape.SetRandomShape();
    CurrentPosition = { Width / 2, Height - 1 };
}

bool AMainBoard::CanMove(const FIntPoint& NewPos, const TArray<FIntPoint>& Blocks) const
{
    for (auto Block : Blocks)
    {
        FIntPoint P = NewPos + Block;
        if (P.X < 0 || P.X >= Width || P.Y < 0 || P.Y >= Height || Grid[P.X][P.Y])
            return false;
    }
    return true;
}

void AMainBoard::LockShape()
{
    for (auto Block : CurrentShape.GetBlocks())
    {
        FIntPoint P = CurrentPosition + Block;
        if (P.X >= 0 && P.X < Width && P.Y >= 0 && P.Y < Height)
            Grid[P.X][P.Y] = true;
    }

    ClearFullLines();
    SpawnNewShape();
    UpdateVisualBlocks();
}

void AMainBoard::MoveLeft()
{
    FIntPoint NewPos = CurrentPosition + FIntPoint(-1, 0);
    if (CanMove(NewPos, CurrentShape.GetBlocks()))
        CurrentPosition = NewPos;
}

void AMainBoard::MoveRight()
{
    FIntPoint NewPos = CurrentPosition + FIntPoint(1, 0);
    if (CanMove(NewPos, CurrentShape.GetBlocks()))
        CurrentPosition = NewPos;
}

void AMainBoard::MoveDown()
{
    FIntPoint NewPos = CurrentPosition + FIntPoint(0, -1);
    if (CanMove(NewPos, CurrentShape.GetBlocks()))
        CurrentPosition = NewPos;
    else
        LockShape();
}

void AMainBoard::Rotate()
{
    auto Next = CurrentShape.GetNextRotationBlocks();
    if (CanMove(CurrentPosition, Next))
        CurrentShape.Rotate();
}



void AMainBoard::SpawnWall()
{
    // 왼쪽/오른쪽 벽
    for (int y = 0; y < Height; ++y)
    {
        SpawnWallBlockAt(FIntPoint(-1, y));             // 왼쪽
        SpawnWallBlockAt(FIntPoint(Width, y));          // 오른쪽
    }

    // 바닥 벽
    for (int x = -1; x <= Width; ++x)
    {
        SpawnWallBlockAt(FIntPoint(x, -1));             // 바닥
    }
}

void AMainBoard::SpawnWallBlockAt(FIntPoint GridPos)
{
    if (!WallBlockClass) return;

    FVector Origin = GetActorLocation();
    FVector SpawnLocation = Origin + FVector(0, GridPos.X * CellSize, GridPos.Y * CellSize);
    FActorSpawnParameters Params;

    GetWorld()->SpawnActor<AActor>(WallBlockClass, SpawnLocation, FRotator::ZeroRotator, Params);
}


void AMainBoard::UpdateVisualBlocks()
{
    if (!BlockClass) return;

    FVector Origin = GetActorLocation();
    float Size = CellSize;

    // 기존 블록 제거
    for (auto* Block : StaticBlocks)
        if (Block) Block->Destroy();
    StaticBlocks.Empty();

    for (auto* Block : ActiveBlocks)
        if (Block) Block->Destroy();
    ActiveBlocks.Empty();

    // 고정된 블록 생성
    for (int x = 0; x < Width; x++)
    {
        for (int y = 0; y < Height; y++)
        {
            if (Grid[x][y])
            {
                FVector Pos = Origin + FVector(0, x * Size, y * Size);
                FActorSpawnParameters Params;
                AActor* Block = GetWorld()->SpawnActor<AActor>(BlockClass, Pos, FRotator::ZeroRotator, Params);
                if (Block)
                    StaticBlocks.Add(Block);
            }
        }
    }

    

    // 현재 움직이는 블록 생성
    for (auto Offset : CurrentShape.GetBlocks())
    {
        FIntPoint Point = CurrentPosition + Offset;
        FVector Pos = Origin + FVector(0, Point.X * Size, Point.Y * Size);
        FActorSpawnParameters Params;
        UMaterialInterface* ShapeMat = GetMaterialForShape(CurrentShape.GetType());
        AActor* Block = GetWorld()->SpawnActor<AActor>(BlockClass, Pos, FRotator::ZeroRotator, Params);

        if (Block)
        {
            ActiveBlocks.Add(Block);
        }
    }
}


void AMainBoard::ClearFullLines()
{
    int LinesCleared = 0;

    for (int y = 0; y < Height; ++y)
    {
        bool bIsFullLine = true;
        for (int x = 0; x < Width; ++x)
        {
            if (!Grid[x][y])
            {
                bIsFullLine = false;
                break;
            }
        }

        if (bIsFullLine)
        {
            for (int y2 = y; y2 < Height - 1; ++y2)
            {
                for (int x = 0; x < Width; ++x)
                {
                    Grid[x][y2] = Grid[x][y2 + 1];
                }
            }

            for (int x = 0; x < Width; ++x)
            {
                Grid[x][Height - 1] = false;
            }

            --y;
            ++LinesCleared;
        }
    }

    if (LinesCleared > 0)
    {
        // 시각적으로도 갱신
        UpdateVisualBlocks();

        // 점수 계산 등도 여기에 추가 가능
    }
}

UMaterialInterface* AMainBoard::GetMaterialForShape(EShapeType Type)
{
    switch (Type)
    {
    case EShapeType::I: return Material_I;
    case EShapeType::O: return Material_O;
    case EShapeType::T: return Material_T;
    case EShapeType::S: return Material_S;
    case EShapeType::Z: return Material_Z;
    case EShapeType::L: return Material_L;
    case EShapeType::J: return Material_J;
    default: return nullptr;
    }
}