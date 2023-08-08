// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AlphabetGameMode.h"
#include "AlphabetCharacter.h"
#include "AlphabetPlayerState.h"
#include "AlphabetPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

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

    GetWorldTimerManager().SetTimer(
        RestartPlayerHandle,
        [this, NewPlayer]()
        {
            if (!IsValid(NewPlayer->GetPawn())) return;

            NewPlayer->GetPawn()->OnDestroyed.AddDynamic(this, &AAlphabetGameMode::OnPlayerDestroyed);
            GetWorldTimerManager().ClearTimer(this->RestartPlayerHandle);
        },
        0.1f, true);
}

void AAlphabetGameMode::RestartPlayer(AController* NewPlayer)
{
    Super::RestartPlayer(NewPlayer);

    GetWorldTimerManager().SetTimer(
        RestartPlayerHandle,
        [this, NewPlayer]()
        {
            if (!IsValid(NewPlayer->GetPawn())) return;

            NewPlayer->GetPawn()->OnDestroyed.AddDynamic(this, &AAlphabetGameMode::OnPlayerDestroyed);
            GetWorldTimerManager().ClearTimer(this->RestartPlayerHandle);
        },
        0.1f, true);
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

    if (PlayerState->bIsABot)
    {
        auto Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        if (!Player) return;

        if (IAlphabetPlayerStateInterface* Interface = Cast<IAlphabetPlayerStateInterface>(Player->GetPlayerState()))
        {
            Interface->Execute_SetScore(Player->GetPlayerState(), Player->GetPlayerState()->Score + 1.f);
        }
    }
}

void AAlphabetGameMode::OnPlayerDestroyed(AActor* DestroyedActor)
{
    RestartPlayer(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    OnPlayerDestroyedBlueprint(DestroyedActor);
}
