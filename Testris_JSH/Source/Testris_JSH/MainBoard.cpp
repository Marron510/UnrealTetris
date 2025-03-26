// Fill out your copyright notice in the Description page of Project Settings.


#include "MainBoard.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMainBoard::AMainBoard()
{
	PrimaryActorTick.bCanEverTick = true;
    CurrentPosition = { Width / 2, Height - 1 };

    // ī�޶� 
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpring"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom);
    FollowCamera->bUsePawnControlRotation = false;
    FollowCamera->SetRelativeLocation(FVector{ WorldLocationX, Width * -0.5f * CellSize , Height * 0.5f * CellSize });


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
    }


    UpdateVisualBlocks();
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

    SpawnNewShape();
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

void AMainBoard::DrawDebug()
{
    FVector Origin = GetActorLocation();
    float Size = 50.0f;

    // Draw locked grid
    for (int x = 0; x < Width; x++)
    {
        for (int y = 0; y < Height; y++)
        {
            if (Grid[x][y])
            {
                FVector Pos = Origin + FVector(0, x * Size, y * Size);
                DrawDebugBox(GetWorld(), Pos, FVector(20), FColor::Red);
            }
        }
    }

    // Draw current shape
    for (auto P : CurrentShape.GetBlocks())
    {
        FIntPoint Point = CurrentPosition + P;
        FVector Pos = Origin + FVector(0, Point.X * Size, Point.Y * Size);
        DrawDebugBox(GetWorld(), Pos, FVector(20), FColor::Green);
    }
}

void AMainBoard::SpawnWall()
{
    // ����/������ ��
    for (int y = 0; y < Height; ++y)
    {
        SpawnWallBlockAt(FIntPoint(-1, y));             // ����
        SpawnWallBlockAt(FIntPoint(Width, y));          // ������
    }

    // �ٴ� ��
    for (int x = -1; x <= Width; ++x)
    {
        SpawnWallBlockAt(FIntPoint(x, -1));             // �ٴ�
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

    // ���� ��� ����
    for (auto* Block : StaticBlocks)
        if (Block) Block->Destroy();
    StaticBlocks.Empty();

    for (auto* Block : ActiveBlocks)
        if (Block) Block->Destroy();
    ActiveBlocks.Empty();

    // ������ ��� ����
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

    // ���� �����̴� ��� ����
    for (auto Offset : CurrentShape.GetBlocks())
    {
        FIntPoint Point = CurrentPosition + Offset;
        FVector Pos = Origin + FVector(0, Point.X * Size, Point.Y * Size);
        FActorSpawnParameters Params;
        AActor* Block = GetWorld()->SpawnActor<AActor>(BlockClass, Pos, FRotator::ZeroRotator, Params);
        if (Block)
        {
            ActiveBlocks.Add(Block);
        }
    }
}
