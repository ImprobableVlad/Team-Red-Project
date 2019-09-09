// Fill out your copyright notice in the Description page of Project Settings.


#include "MountainActor.h"
#include "Engine.h"

// Sets default values
AMountainActor::AMountainActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMountainActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMountainActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMountainActor::generateMountain(int32 seed) {
	FRandomStream RandomNumberGenerator;
	RandomNumberGenerator.Initialize(seed);
	TArray<TArray<int32>> heights;
	TArray<int32> row;
	row.Init(0, size);
	heights.Init(row, size);
	for (int32 i = 0; i < size; i++) {
		for (int32 j = 0; j < size; j++) {
			heights[i][j] = RandomNumberGenerator.FRandRange(0, 10);

			// generate cube for this place
			FString cubeName = FString("Cube");
			cubeName.AppendInt(i);
			cubeName.Append("_");
			cubeName.AppendInt(j);
			UStaticMeshComponent* CubeComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName(*cubeName)); // UStaticMeshComponent
			static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
			if (CubeVisualAsset.Succeeded())
			{
				CubeComponent->SetStaticMesh(CubeVisualAsset.Object);
				CubeComponent->SetRelativeScale3D(FVector(1, heights[i][j], 1));
				CubeComponent->SetRelativeLocation(FVector(i, 0, j));
				CubeComponent->SetupAttachment(RootComponent);
			}
		}
	}
}
