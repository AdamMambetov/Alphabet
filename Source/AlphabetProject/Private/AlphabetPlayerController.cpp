// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphabetPlayerController.h"
#include "GameFramework/Character.h"

AAlphabetPlayerController::AAlphabetPlayerController() : Super() {}

void AAlphabetPlayerController::OnPossess(APawn* aPawn)
{
    Super::OnPossess(aPawn);
    OnPossessedPawnHandler.Broadcast(aPawn);
}
