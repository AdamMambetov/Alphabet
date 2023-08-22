// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AlphabetGameMode.h"
#include "AlphabetCharacter.h"
#include "AlphabetPlayerState.h"
#include "AlphabetPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "GameFramework/SpectatorPawn.h"

AAlphabetGameMode::AAlphabetGameMode()
{
    // GameStateClass
    PlayerControllerClass = AAlphabetPlayerController::StaticClass();
    PlayerStateClass = AAlphabetPlayerState::StaticClass();
    DefaultPawnClass = AAlphabetCharacter::StaticClass();
}

void AAlphabetGameMode::RestartPlayer(AController* NewPlayer)
{
    if (IsValid(NewPlayer->GetPawn()))
    {
        auto OldPawn = NewPlayer->GetPawn();
        NewPlayer->PawnPendingDestroy(OldPawn);
        OldPawn->Destroy();
    }

    Super::RestartPlayer(NewPlayer);

    if (bIsPlayerSpawn)
    {
        auto PlayerState = NewPlayer->GetPlayerState<AAlphabetPlayerState>();
        PlayerState->SetLives(PlayerState->GetLives() - 1);
    }
    bIsPlayerSpawn = true;
}

APawn* AAlphabetGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
    const FTransform L_SpawnTransform{StartSpot->GetActorRotation(), StartSpot->GetActorLocation()};
    const auto L_CollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    APawn* ReturnValue = GetWorld()->SpawnActorDeferred<APawn>(SelectPlayerPawnClass().Get(), L_SpawnTransform, NewPlayer, //
        Instigator, L_CollisionHandlingMethod);
    UGameplayStatics::FinishSpawningActor(ReturnValue, L_SpawnTransform);

    return ReturnValue;
}

TSubclassOf<APawn> AAlphabetGameMode::SelectPlayerPawnClass_Implementation()
{
    return DefaultPawnClass;
}

void AAlphabetGameMode::PawnDead_Implementation(class APlayerState* PlayerState)
{
    if (!PlayerState) return;

    auto Player = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (PlayerState->bIsABot)
    {
        if (!Player) return;

        if (IAlphabetPlayerStateInterface* Interface = Cast<IAlphabetPlayerStateInterface>(Player->GetPlayerState<APlayerState>()))
        {
            Interface->Execute_SetScore(Player->GetPlayerState<APlayerState>(), Player->GetPlayerState<APlayerState>()->Score + 1.f);
        }
    }
    else
    {
        auto Spectator = GetWorld()->SpawnActor<APawn>(SpectatorClass.Get(), PlayerState->GetPawn()->GetActorTransform());
        Player->Possess(Spectator);
        Spectator->DisableInput(Player);
    }
}

bool AAlphabetGameMode::PlayerCanRestart_Implementation(APlayerController* Player)
{
    return Player->GetPlayerState<AAlphabetPlayerState>()->GetLives() > 1;
}

void AAlphabetGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
    if (!bStartPlayersAsSpectators && !MustSpectate(NewPlayer))
    {
        RestartPlayer(NewPlayer);
    }
}
