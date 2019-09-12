// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DetachablePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TEAMREDPROJECT_API ADetachablePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void BecomeSpectator();
	UFUNCTION(BlueprintCallable)
	void StopSpectating();
};
