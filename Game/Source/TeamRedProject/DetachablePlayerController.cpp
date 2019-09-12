// Fill out your copyright notice in the Description page of Project Settings.


#include "DetachablePlayerController.h"
#include "GameFramework/PlayerState.h"

void ADetachablePlayerController::BecomeSpectator()
{
	ChangeState(NAME_Spectating);
	if (Role == ROLE_Authority && PlayerState != NULL)
	{
		PlayerState->bIsSpectator = true;
		PlayerState->bOnlySpectator = true;
	}
}

void ADetachablePlayerController::StopSpectating()
{
	ChangeState(NAME_Playing);
	if (Role == ROLE_Authority && PlayerState != NULL)
	{
		PlayerState->bIsSpectator = false;
		PlayerState->bOnlySpectator = false;
	}
}
