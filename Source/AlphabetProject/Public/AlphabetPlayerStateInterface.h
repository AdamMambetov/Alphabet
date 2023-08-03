// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AlphabetPlayerStateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UAlphabetPlayerStateInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class ALPHABETPROJECT_API IAlphabetPlayerStateInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Alphabet | PlayerStateInterface")
    void SetScore(const float NewScore);
};
