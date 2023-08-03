// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphabetPlayerState.h"
#include "AlphabetStatsComponent.h"
#include "AlphabetPlayerController.h"
#include "GameFramework/Pawn.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

AAlphabetPlayerState::AAlphabetPlayerState() {}

void AAlphabetPlayerState::BeginPlay()
{
    Super::BeginPlay();
    Init();
}

void AAlphabetPlayerState::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AAlphabetPlayerState::Init_Implementation()
{
    if (!IsValid(GetPawn())) return;
    auto Controller = Cast<AAlphabetPlayerController>(GetPawn()->GetController());
    if (!IsValid(Controller)) return;
    Controller->OnPossessedPawnHandler.AddDynamic(this, &AAlphabetPlayerState::OnNewPawnEvent);

    auto StatsComponent = GetPawn()->FindComponentByClass<UAlphabetStatsComponent>();
    if (IsValid(StatsComponent))
    {
        PawnStats = StatsComponent->GetStats();
        StatsComponent->OnStatChangedHandle.AddDynamic(this, &AAlphabetPlayerState::OnPawnStatChange);
    }
}

void AAlphabetPlayerState::OnPawnStatChange(FStatInfo StatInfo)
{
    auto FindStat = PawnStats.FindByPredicate([&StatInfo](FStatInfo a) { return a.Id == StatInfo.Id; });
    if (FindStat)
    {
        *(FindStat) = StatInfo;
    }
    else
    {
        PawnStats = GetPawn()->FindComponentByClass<UAlphabetStatsComponent>()->GetStats();
    }
}

void AAlphabetPlayerState::OnNewPawnEvent(APawn* aPawn)
{
    auto StatsComponent = aPawn->FindComponentByClass<UAlphabetStatsComponent>();
    if (!StatsComponent) return;

    StatsComponent->OnStatChangedHandle.AddDynamic(this, &AAlphabetPlayerState::OnPawnStatChange);
    PawnStats = StatsComponent->GetStats();
}
