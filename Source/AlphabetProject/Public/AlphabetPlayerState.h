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

    UFUNCTION(BlueprintGetter)
    int32 GetLives() const { return Lives; }

    UFUNCTION(BlueprintSetter)
    void SetLives(int32 NewLives) { Lives = NewLives; }

    UFUNCTION(BlueprintGetter)
    bool GetMovement() const { return bMovement; }

    UFUNCTION(BlueprintSetter)
    void SetMovement(bool NewMovement) { bMovement = NewMovement; }

    // AAlphabetPlayerState Functions End

private:
    // AAlphabetPlayerState Variables Begin

    UPROPERTY(VisibleDefaultsOnly, BlueprintGetter = GetPawnStats, Category = "Alphabet | PlayerState", //
        meta = (AllowPrivateAccess = "true"))
    TArray<FStatInfo> PawnStats;

    UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetLives, BlueprintSetter = SetLives, Category = "Alphabet | PlayerState", //
        meta = (AllowPrivateAccess = "true", ClampMin = 1, UIMin = 1))
    int32 Lives = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetMovement, BlueprintSetter = SetMovement, Category = "Alphabet | PlayerState", //
        meta = (AllowPrivateAccess = "true"))
    bool bMovement = true;

    // AAlphabetPlayerState Variables End
};
