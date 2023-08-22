// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AlphabetGameModeInterface.h"
#include "AlphabetGameMode.generated.h"

UCLASS(MinimalApi)
class AAlphabetGameMode : public AGameModeBase, public IAlphabetGameModeInterface
{
    GENERATED_BODY()

public:
    // Default Constructor
    AAlphabetGameMode();

public:
    // AGameModeBase Begin

    virtual void RestartPlayer(AController* NewPlayer);

    APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot);

    bool PlayerCanRestart_Implementation(APlayerController* Player);

    void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer);

    // AGameModeBase End

public:
    // IAlphabetGameModeInterface Begin

    void PawnDead_Implementation(class APlayerState* PlayerState);

    // IAlphabetGameModeInterface Begin

protected:
    // AGameModeBase Functions Begin

    UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Alphabet | GameMode")
    TSubclassOf<APawn> SelectPlayerPawnClass();
    TSubclassOf<APawn> SelectPlayerPawnClass_Implementation();

    // AGameModeBase Functions End

private:
    // AGameModeBase Variables Begin

    bool bIsPlayerSpawn = false;

    // AGameModeBase Variables End
};
