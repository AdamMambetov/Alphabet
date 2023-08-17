//

#include "AlphabetNpcSpawner.h"
#include "NavigationInvokerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RecastNavMesh.h"
#include "TimerManager.h"
#include "NavigationSystem.h"
#include "ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "CharacterInfo.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

AAlphabetNpcSpawner::AAlphabetNpcSpawner()
{
    NavigationInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvoker"));

    static ConstructorHelpers::FObjectFinder<UDataTable> L_EnemiesDataTable(
        TEXT("DataTable'/Game/Alphabet/Core/Engine/DT_Enemies.DT_Enemies'"));
    if (L_EnemiesDataTable.Succeeded())
    {
        EnemiesDataTable = L_EnemiesDataTable.Object;
    }
}

void AAlphabetNpcSpawner::BeginPlay()
{
    Super::BeginPlay();

    NavMesh = Cast<ARecastNavMesh>(UGameplayStatics::GetActorOfClass(GetWorld(), ARecastNavMesh::StaticClass()));
    NpcToSpawnCount = FMath::RandRange(MinAmountOfSpawnNpc, MaxAmountOfSpawnNpc);

    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AAlphabetNpcSpawner::SpawnNpc, SpawnInterval, true, 1.f);
}

void AAlphabetNpcSpawner::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AAlphabetNpcSpawner::SpawnNpc()
{
    FNavLocation L_ResultLocation;

    if (SpawnedNpcs.Num() >= NpcToSpawnCount)
    {
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
        return;
    }

    auto L_NavigationSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
    if (!L_NavigationSystem) return;

    bool L_FindLocation = L_NavigationSystem->GetRandomPointInNavigableRadius(GetActorLocation(), SpawnRadius, L_ResultLocation);
    if (!L_FindLocation) return;

    const FTransform L_SpawnTransform{FRotator::ZeroRotator, L_ResultLocation.Location};
    const auto L_CollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    auto L_SpawnedNpc = GetWorld()->SpawnActorDeferred<APawn>(SelectNpcClass(), L_SpawnTransform, nullptr, //
        nullptr, L_CollisionHandlingMethod);
    UGameplayStatics::FinishSpawningActor(L_SpawnedNpc, L_SpawnTransform);

    L_SpawnedNpc->OnDestroyed.AddDynamic(this, &AAlphabetNpcSpawner::OnNpcDestroy);

    SpawnedNpcs.Add(L_SpawnedNpc);
    if (!GetWorldTimerManager().IsTimerActive(InitNpcTimerHandle))
    {
        GetWorldTimerManager().SetTimer(
            InitNpcTimerHandle,
            [this]()
            {
                bool InitSuccess = true;
                for (auto L_SpawnedNpc : SpawnedNpcs)
                {
                    if (!IsValid(L_SpawnedNpc->GetController()))
                    {
                        InitSuccess = false;
                        return;
                    }

                    auto AiController = Cast<AAIController>(L_SpawnedNpc->GetController());
                    if (!IsValid(AiController))
                    {
                        InitSuccess = false;
                        return;
                    }

                    if (!IsValid(AiController->GetBlackboardComponent()))
                    {
                        InitSuccess = false;
                        return;
                    }
                    auto Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
                    if (AiController->GetBlackboardComponent()->GetValueAsObject(TEXT("EnemyActor")) != Player)
                        AiController->GetBlackboardComponent()->SetValueAsObject(TEXT("EnemyActor"), Player);
                }
                if (InitSuccess) GetWorldTimerManager().ClearTimer(InitNpcTimerHandle);
            },
            0.1f, true);
    }
}

void AAlphabetNpcSpawner::ResetSpawner()
{
    if (SpawnedNpcs.Num() != 0)
    {
        for (int32 i = SpawnedNpcs.Num() - 1; i != -1; --i)
        {
            SpawnedNpcs[i]->Destroy();
        }
    }

    NpcToSpawnCount = FMath::RandRange(MinAmountOfSpawnNpc, MaxAmountOfSpawnNpc);

    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AAlphabetNpcSpawner::SpawnNpc, SpawnInterval, true, 1.f);
}

UClass* AAlphabetNpcSpawner::SelectNpcClass()
{
    TArray<FGameplayTag> L_NpcArray;
    Npcs.GetGameplayTagArray(L_NpcArray);

    if (L_NpcArray.Num() == 0 || L_NpcArray[0].MatchesTagExact(FGameplayTag::RequestGameplayTag(TEXT("Character"), false))) return nullptr;

    auto L_SelectedNpcId = L_NpcArray[FMath::RandRange(0, L_NpcArray.Num() - 1)];

    FString Context;
    auto L_FindRow = EnemiesDataTable->FindRow<FCharacterInfo>(L_SelectedNpcId.GetTagName(), Context);
    if (!L_FindRow) return nullptr;

    return L_FindRow->Class;
}

void AAlphabetNpcSpawner::OnNpcDestroy(AActor* DestroyedNpc)
{
    SpawnedNpcs.Remove(Cast<APawn>(DestroyedNpc));
}
