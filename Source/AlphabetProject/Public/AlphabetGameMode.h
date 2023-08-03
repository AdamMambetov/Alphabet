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

    virtual void PostLogin(APlayerController* NewPlayer) override;

    virtual void RestartPlayer(AController* NewPlayer);

    // AGameModeBase End

public:
    // IAlphabetGameModeInterface Begin

    void PawnDead_Implementation(class APlayerState* PlayerState);

    // IAlphabetGameModeInterface Begin

protected:
    // AGameModeBase Functions Begin

    UFUNCTION()
    virtual void OnPlayerDestroyed(AActor* DestroyedActor);

    UFUNCTION(BlueprintImplementableEvent, Category = "Alphabet | GameMode", meta = (DisplayName = "On Player Destroyed"))
    void OnPlayerDestroyedBlueprint(AActor* DestroyedActor);

    // AGameModeBase Functions End
};
