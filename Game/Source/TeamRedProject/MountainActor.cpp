// Fill out your copyright notice in the Description page of Project Settings.


#include "MountainActor.h"
#include "Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMountainActor::AMountainActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;  // allows component replication
	
	InstancedComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/Geometry/Meshes/MountainCube.MountainCube"));
	if (CubeVisualAsset.Succeeded())
	{
		InstancedComponent->SetStaticMesh(CubeVisualAsset.Object);
		InstancedComponent->SetupAttachment(RootComponent);
	}
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

void AMountainActor::updateHeight(int32 i, int32 j, TArray<TArray<int32>>* heights, FRandomStream* rs) {
	
	int32 maxHeight = -9999;
	int di[] = { -1, 0, 1, 1, 1, 0,-1,-1 };
	int dj[] = { -1,-1,-1, 0, 1, 1, 1, 0 };
	for (int32 d = 0; d < 8; d++) {
		int32 ti = i+di[d];
		int32 tj = j+dj[d];
		if(ti >= 0 && ti < Width && tj >= 0 && tj < Width)
			maxHeight = FMath::Max(maxHeight, (*heights)[ti][tj]);
	}
	float roll = (*rs).FRandRange(0, 1);
	if (roll < Drop1Prob) {
		(*heights)[i][j] = maxHeight - 1;
	}
	else if (roll < Drop1Prob + Drop2Prob) {
		(*heights)[i][j] = maxHeight - 2;
	}
	else {
		(*heights)[i][j] = maxHeight;
	}
}

void AMountainActor::UpdateMountain() {
	InstancedComponent->ClearInstances();

	// mountain needs a center
	if (Width % 2 == 0)
		Width++;

	// initialize variables
	FRandomStream rs;
	rs.Initialize(Seed);
	TArray<TArray<int32>> heights;
	TArray<int32> row;
	row.Init(-9999, Width);
	heights.Init(row, Width);

	// generate height field
	heights[Width / 2][Width / 2] = Height;

	for (int32 offset = 1; offset <= Width / 2; offset++) {
		for (int32 j = Width / 2 - offset + 1; j <= Width / 2 + offset - 1; j++) {
			updateHeight(Width / 2 + offset, j, &heights, &rs);
			updateHeight(Width / 2 - offset, j, &heights, &rs);
		}
		for (int32 i = Width / 2 - offset; i <= Width / 2 + offset; i++) {
			updateHeight(i, Width / 2 + offset, &heights, &rs);
			updateHeight(i, Width / 2 - offset, &heights, &rs);
		}
	}

	// generate surrounding plateaus
	for (int32 i = 0; i < Width; i++) {
		for (int32 j = 0; j < Width; j++) {
			int32 h = heights[i][j];
			if (h < 1) {
				if (h > -PlateauSize * PlateauAmount) {
					heights[i][j] = PlateauAmount - (-h / PlateauSize);
				}
			}
			else heights[i][j] += PlateauAmount;
		}
	}

	// smooth mountain
	TArray<TArray<bool>> fillMask;
	TArray<bool> fillRow;
	TArray<FVector2D> floodStack;
	int32 di[] = {0, 1, 0, -1};
	int32 dj[] = {1, 0, -1, 0};
	for (int32 h = 1; h < Height + PlateauAmount; h++) {
		// reinit
		fillRow.Init(false, Width);
		fillMask.Init(fillRow, Width);
		floodStack.Empty();
		fillMask[0][0] = true;
		floodStack.Add(FVector2D(0,0));
		// floodfill from ouside to inside
		while (floodStack.Num()) {
			FVector2D pos = floodStack.Pop();
			for (int32 k = 0; k < 4; k++) {
				int32 ti = pos.X + di[k];
				int32 tj = pos.Y + dj[k];
				if (ti >= 0 && ti < Width && tj >= 0 && tj < Width) {
					if (!fillMask[ti][tj] && heights[ti][tj] < h) {
						fillMask[ti][tj] = true;
						floodStack.Add(FVector2D(ti, tj));
					}
				}
			}
		}
		// set all unflooded tiles lower than h to to h
		for (int32 i = 0; i < Width; i++) {
			for (int32 j = 0; j < Width; j++) {
				if (!fillMask[i][j] && heights[i][j] < h) {
					heights[i][j] = h;
				}
			}
		}
	}

	// create mountain columns based on heightfield
	for (int32 i = 0; i < Width; i++) {
		for (int32 j = 0; j < Width; j++) {
			if (heights[i][j] <= 0)
				continue;
			FTransform cubeTransform;
			FVector originOffset = FVector(-100*Width*0.5, -100 * Width*0.5, 0);
			cubeTransform.SetLocation(FVector(100 * i, 100 * j, 100* heights[i][j]*0.5f) + originOffset);
			cubeTransform.SetScale3D(FVector(1, 1, heights[i][j]));
			InstancedComponent->AddInstance(cubeTransform);
			if (heights[i][j] == 1)
				BeachTops.Add((RootComponent->RelativeLocation + FVector(100 * i, 100 * j, 100 * heights[i][j]) + originOffset) * RootComponent->RelativeScale3D);
		}
	}

	// Register InstancedStaticMeshComponent to generate NavMesh
	UpdateComponentTransforms();
}

void AMountainActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMountainActor, Seed);
}

