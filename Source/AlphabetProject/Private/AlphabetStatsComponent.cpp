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
    bDeath = false;
    FStatInfo HealthStatInfo;
    GetStatInfo(UDefaults::MakeTag("Stat.Health"), HealthStatInfo);
    FStatModifier Modifier("Stat.Health", HealthStatInfo.MaxValue - HealthStatInfo.CurrentValue, "StatModifier.Value.Current");
    Server_ModifyStat(Modifier);
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
