// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AlphabetPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPossessedPawn, APawn*, NewActor);

/**
 *
 */
UCLASS()
class ALPHABETPROJECT_API AAlphabetPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    // Default Constructor
    AAlphabetPlayerController();

    FOnPossessedPawn OnPossessedPawnHandler;

protected:
    virtual void OnPossess(APawn* aPawn) override;
};
