//

#include "AlphabetStatsComponent.h"
#include "GameFramework/Actor.h"

void UAlphabetStatsComponent::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(GetOwner()))
    {
        GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UAlphabetStatsComponent::OnOwnerTakeAnyDamage);
    }
}

void UAlphabetStatsComponent::Revive()
{
    if (!bDeath) return;

    for (auto& Stat : Stats)
    {
        Stat.CurrentValue = Stat.MaxValue;
    }
    bDeath = false;
}

void UAlphabetStatsComponent::OnOwnerTakeAnyDamage( //
    AActor* DamagedActor,                           //
    float Damage,                                   //
    const class UDamageType* DamageType,            //
    class AController* InstigatedBy,                //
    AActor* DamageCauser)                           //
{
    FStatModifier Modifier("Stat.Health", Damage * -1.f, "StatModifier.Value.Current");
    Server_ModifyStat(Modifier);

    OnOwnerTakeAnyDamageBlueprint(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
}
