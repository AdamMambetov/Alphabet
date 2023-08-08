// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AlphabetGameMode.h"
#include "AlphabetCharacter.h"
#include "AlphabetPlayerState.h"
#include "AlphabetPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

AAlphabetGameMode::AAlphabetGameMode()
{
    // GameStateClass
    PlayerControllerClass = AAlphabetPlayerController::StaticClass();
    PlayerStateClass = AAlphabetPlayerState::StaticClass();
    DefaultPawnClass = AAlphabetCharacter::StaticClass();
}

void AAlphabetGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    RestartPlayer(NewPlayer);
}

void AAlphabetGameMode::RestartPlayer(AController* NewPlayer)
{
    Super::RestartPlayer(NewPlayer);
    NewPlayer->GetPawn()->OnDestroyed.AddDynamic(this, &AAlphabetGameMode::OnPlayerDestroyed);
}

void AAlphabetGameMode::PawnDead_Implementation(class APlayerState* PlayerState)
{
    if (!PlayerState) return;

    if (PlayerState->bIsABot)
    {
        auto Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        if (!Player) return;

        if (IAlphabetPlayerStateInterface* Interface = Cast<IAlphabetPlayerStateInterface>(Player->GetPlayerState()))
        {
            Interface->Execute_SetScore(Player->GetPlayerState(), Player->GetPlayerState()->Score + 1.f);
        }

        // UEngine::GetFirstGamePlayer();
    }
    else
    {
    }
}

void AAlphabetGameMode::OnPlayerDestroyed(AActor* DestroyedActor)
{
    RestartPlayer(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    OnPlayerDestroyedBlueprint(DestroyedActor);
}
