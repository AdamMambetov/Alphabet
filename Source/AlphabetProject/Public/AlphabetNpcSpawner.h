// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "AlphabetNpcSpawner.generated.h"

class UNavigationInvokerComponent;
class ARecastNavMesh;
class UDataTable;

/**
 *
 */
UCLASS()
class ALPHABETPROJECT_API AAlphabetNpcSpawner : public AActor
{
    GENERATED_BODY()

public:
    // Default Constructor
    AAlphabetNpcSpawner();

public:
    // AActor Begin

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

    // AActor End

public:
    // AAlphabetNpcSpawner Functions Begin

    FGameplayTagContainer GetNpcToSpawn() { return Npcs; }

    UFUNCTION(BlueprintCallable)
    void SpawnNpc();

    UFUNCTION(BlueprintCallable)
    void ResetSpawner();

    UFUNCTION(BlueprintCallable)
    UClass* SelectNpcClass();

    UFUNCTION(BlueprintCallable)
    void InitAllNpcs();

protected:
    UFUNCTION()
    virtual void OnNpcDestroy(AActor* DestroyedNpc);

    UFUNCTION()
    virtual void OnPlayerRespawn(APawn* NewPlayer);

    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Npc Destroy"))
    void OnNpcDestroyBlueprint(AActor* DestroyedNpc);

    // AAlphabetNpcSpawner Functions End

private:
    // AAlphabetNpcSpawner Variables Begin

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
    UNavigationInvokerComponent* NavigationInvoker;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner",
        meta = (AllowPrivateAccess = true, ExposeOnSpawn = true, Categories = "Character"))
    FGameplayTagContainer Npcs;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
    float SpawnInterval = 0.01f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
    float SpawnRadius = 500.f;

    UPROPERTY(BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
    TArray<APawn*> SpawnedNpcs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
    int32 MinAmountOfSpawnNpc;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
    int32 MaxAmountOfSpawnNpc;

    UPROPERTY()
    FTimerHandle SpawnTimerHandle;

    UPROPERTY()
    FTimerHandle InitNpcTimerHandle;

    UPROPERTY()
    int32 NpcToSpawnCount;

    UPROPERTY()
    ARecastNavMesh* NavMesh;

    UPROPERTY()
    UDataTable* EnemiesDataTable;

    // AAlphabetNpcSpawner Variables End
};
