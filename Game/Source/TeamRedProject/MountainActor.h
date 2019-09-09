// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MountainActor.generated.h"

UCLASS()
class TEAMREDPROJECT_API AMountainActor : public AActor
{
	GENERATED_BODY()

private:
	void generateMountain(int32 seed);

public:	
	// Sets default values for this actor's properties
	AMountainActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actor)
	int32 size = 5;

};
