// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AlphabetGameModeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UAlphabetGameModeInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class ALPHABETPROJECT_API IAlphabetGameModeInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Alphabet | AlphabetGameModeInterface")
    void PawnDead(class APlayerState* PlayerState);
};
