// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tetrimo.generated.h"

UCLASS()
class TESTRIS_JSH_API ATetrimo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATetrimo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UStaticMeshComponent* GetMeshComponent()
	{
		return MeshComponent;
	}
public:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

public:

	void InitializeMesh();

};
