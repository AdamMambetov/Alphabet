// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AlphabetPlayerStateInterface.h"
#include "Stats/StatInfo.h"
#include "AlphabetPlayerState.generated.h"

class UAlphabetStatsComponent;

/**
 *
 */
UCLASS()
class ALPHABETPROJECT_API AAlphabetPlayerState : public APlayerState, public IAlphabetPlayerStateInterface
{
    GENERATED_BODY()

public:
    // Default Constructor
    AAlphabetPlayerState();

public:
    // AActor Begin

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    // AActor End

public:
    // IAlphabetPlayerStateInterface Begin

    void SetScore_Implementation(const float NewScore) { Score = NewScore; }

    // IAlphabetPlayerStateInterface End

protected:
    // AAlphabetPlayerState Functions Begin

    UFUNCTION(BlueprintNativeEvent, Category = "Alphabet | PlayerState")
    void Init();
    void Init_Implementation();

    UFUNCTION()
    virtual void OnPawnStatChange(FStatInfo StatInfo);

    UFUNCTION(BlueprintImplementableEvent, Category = "Alphabet | PlayerState", meta = (DisplayName = "On Pawn Take Any Damage"))
    void OnPawnStatChangeBlueprint(FStatInfo StatInfo);

    UFUNCTION()
    virtual void OnNewPawnEvent(APawn* NewPawn);

public:
    UFUNCTION(BlueprintGetter)
    TArray<FStatInfo> GetPawnStats() { return PawnStats; }

    // AAlphabetPlayerState Functions End

private:
    // AAlphabetPlayerState Variables Begin

    UPROPERTY(VisibleDefaultsOnly, BlueprintGetter = GetPawnStats, Category = "Alphabet | PlayerState", //
        meta = (AllowPrivateAccess = "true"))
    TArray<FStatInfo> PawnStats;

    // AAlphabetPlayerState Variables End
};
