//

#include "AlphabetAttackComponent.h"
#include "Core/Defaults.h"
#include "GameFramework/Character.h"

UAlphabetAttackComponent::UAlphabetAttackComponent() {}

void UAlphabetAttackComponent::BeginPlay()
{
    Super::BeginPlay();

    OnSaveCombo.AddDynamic(this, &UAlphabetAttackComponent::OnSaveComboEvent);
    OnResetCombo.AddDynamic(this, &UAlphabetAttackComponent::OnResetComboEvent);
}

void UAlphabetAttackComponent::AttackById(const FGameplayTag AttackId)
{
    NewAttackId = AttackId;
    if (!CanAttack()) return;
    if (UDefaults::GetTagParents(NewAttackId).Last(/*IndexFromTheEnd*/ 1) != CurrentCharacterId) return;

    if (Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() != CurrentAttackInfo.Id.IsValid()) return;

    const auto L_NewAttackInfo = *AttacksDataTable->FindRow<FAttackInfo>(NewAttackId.GetTagName(), "");
    CurrentAttackInfo = L_NewAttackInfo;
    AttackState = EAttackState::Attack;
    OnAttackStart.Broadcast(CurrentAttackInfo);
}

void UAlphabetAttackComponent::Attack()
{
    switch (AttackState)
    {
    case EAttackState::None:
        AttackById(GetNextAttackId());
        break;
    case EAttackState::Attack:
        AttackState = bIsRandomAttack ? AttackState : EAttackState::SaveCombo;
        break;
    case EAttackState::SaveCombo:
        // if ()
        break;
    }
}

FGameplayTag UAlphabetAttackComponent::GetNextAttackId()
{
    FGameplayTag ReturnValue = FGameplayTag::EmptyTag;

    auto Tags = UDefaults::GetChildrensByTag(CurrentCharacterId);
    if (Tags.Num() <= 1 || !IsValid(AttacksDataTable)) return ReturnValue;

    // Select next attack tag
    if (bIsRandomAttack)
        ReturnValue = Tags[FMath::RandRange(1, Tags.Num() - 1)];
    else
        ReturnValue = Tags.IsValidIndex(ComboCount + 1) ? Tags[ComboCount + 1] : FGameplayTag::EmptyTag;

    if (!ReturnValue.IsValid()) return ReturnValue;

    // Select attack tag by CollisionLocation (Left or Right)
    int32 ChildrenIndex = ((int32)CollisionLocation) + 1;
    ReturnValue = UDefaults::GetChildrensByTag(ReturnValue)[ChildrenIndex];

    return ReturnValue;
}

void UAlphabetAttackComponent::OnSaveComboEvent()
{
    if (AttackState != EAttackState::SaveCombo) return;
    AttackState = EAttackState::None;
    ComboCount++;
    NewAttackId = FGameplayTag::EmptyTag;
    AttackById(GetNextAttackId());
}

void UAlphabetAttackComponent::OnResetComboEvent()
{
    AttackState = EAttackState::None;
    CurrentAttackInfo = FAttackInfo();
    ComboCount = 0;
    NewAttackId = FGameplayTag::EmptyTag;
}
