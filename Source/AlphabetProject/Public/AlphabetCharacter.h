// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/BaseCharacter.h"
#include "Stats/StatInfo.h"
#include "Attack/AttackInfo.h"
#include "AlphabetCharacter.generated.h"

class UAlphabetStatsComponent;
class UAlphabetAttackComponent;
class UBoxComponent;

UCLASS(Config = Game)
class AAlphabetCharacter : public ABaseCharacter
{
    GENERATED_BODY()

public:
    // Default Constructor
    AAlphabetCharacter();

public:
    // AActor Begin

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

    // AActor End

public:
    // APawn Begin

    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    // APawn End

protected:
    // AAlphabetCharacter Functions Begin

    virtual void OnJump();

    virtual void OnStopJumping();

    virtual void OnMoveRight(float AxisValue);

    virtual void OnAttack();

    UFUNCTION()
    virtual void OnStatReachedZero(FStatInfo StatInfo);

    UFUNCTION()
    virtual void OnWeaponCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    virtual void OnAttackStart(FAttackInfo AttackInfo);

    UFUNCTION()
    virtual void OnAttackEnd(FAttackInfo AttackInfo);

    UFUNCTION()
    virtual void OnResetCombo();

    UFUNCTION(BlueprintImplementableEvent, Category = "Alphabet | Character", meta = (DisplayName = "On Jump"))
    void OnJumpBlueprint();

    UFUNCTION(BlueprintImplementableEvent, Category = "Alphabet | Character", meta = (DisplayName = "On Stop Jumping"))
    void OnStopJumpingBlueprint();

    UFUNCTION(BlueprintImplementableEvent, Category = "Alphabet | Character", meta = (DisplayName = "On Move Right"))
    void OnMoveRightBlueprint(float AxisValue);

    UFUNCTION(BlueprintImplementableEvent, Category = "Alphabet | Character", meta = (DisplayName = "On Attack"))
    void OnAttackBlueprint();

    UFUNCTION(BlueprintImplementableEvent, Category = "Alphabet | Character", meta = (DisplayName = "On Stat Reached Zero"))
    void OnStatReachedZeroBlueprint(FStatInfo StatInfo);

    // AAlphabetCharacter Functions End

private:
    // AAlphabetCharacter Variables Begin

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Alphabet | Character", meta = (AllowPrivateAccess = "true"))
    UAlphabetStatsComponent* StatsComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Alphabet | Character", meta = (AllowPrivateAccess = "true"))
    UAlphabetAttackComponent* AttackComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Alphabet | Character", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* WeaponCollision;

    float Direction = 0.f;

    // AAlphabetCharacter Variables End
};
