// Fill out your copyright notice in the Description page of Project Settings.


#include "Tetrimo.h"

// Sets default values
ATetrimo::ATetrimo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CellMesh"));
    RootComponent = MeshComponent;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube"));
    if (CubeMesh.Succeeded())
    {
        MeshComponent->SetStaticMesh(CubeMesh.Object);
        MeshComponent->SetWorldScale3D(FVector(0.5f));
    }
}

// Called when the game starts or when spawned
void ATetrimo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATetrimo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATetrimo::InitializeMesh()
{
}

