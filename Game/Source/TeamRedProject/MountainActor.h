// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MountainActor.generated.h"

UCLASS()
class TEAMREDPROJECT_API AMountainActor : public AActor
{
	GENERATED_BODY()

private:
	void updateHeight(int32 i, int32 j, int32 size, TArray<TArray<int32>>* heights, FRandomStream* rs);

public:	
	// Sets default values for this actor's properties
	AMountainActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UInstancedStaticMeshComponent* InstancedComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MountainParameters)
	int32 Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MountainParameters)
	int32 Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MountainParameters)
	float Drop1Prob;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MountainParameters)
	float Drop2Prob;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = UpdateMountain, Category = MountainParameters)
	int32 Seed;

	UFUNCTION(BlueprintCallable, Category = MountainParameters)
	void UpdateMountain();
};
