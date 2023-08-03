//

#pragma once

#include "CoreMinimal.h"
#include "Stats/StatsComponent.h"
#include "AlphabetStatsComponent.generated.h"

UCLASS()
class ALPHABETPROJECT_API UAlphabetStatsComponent : public UStatsComponent
{
    GENERATED_BODY()

    // UActorComponent Begin

    virtual void BeginPlay() override;

    // UActorComponent End

public:
    // UAlphabetStatsComponent Functions Begin

    UFUNCTION(BlueprintCallable, Category = "Alphabet | StatsComponent")
    void Revive();

protected:
    UFUNCTION()
    virtual void OnOwnerTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, //
        class AController* InstigatedBy, AActor* DamageCauser);

    UFUNCTION(BlueprintImplementableEvent, Category = "Alphabet | PlayerState", meta = (DisplayName = "On Pawn Take Any Damage"))
    void OnOwnerTakeAnyDamageBlueprint(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, //
        class AController* InstigatedBy, AActor* DamageCauser);

    // UAlphabetStatsComponent Functions End
};
