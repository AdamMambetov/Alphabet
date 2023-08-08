//

#pragma once

#include "CoreMinimal.h"
#include "Attack/AttackComponent.h"
#include "AttackCollisionLocation.h"
#include "AlphabetAttackComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveCombo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetCombo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDamageStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDamageEnd);

UCLASS()
class ALPHABETPROJECT_API UAlphabetAttackComponent : public UAttackComponent
{
    GENERATED_BODY()
public:
    // Default Constructor
    UAlphabetAttackComponent();

protected:
    // UActorComponent Begin

    virtual void BeginPlay() override;

    // UActorComponent End

public:
    // UAttackComponent Begin

    virtual void Attack() override;
    virtual void AttackById(const FGameplayTag AttackId) override;

protected:
    virtual FGameplayTag GetNextAttackId() override;

    // UAttackComponent End

protected:
    // UAlphabetAttackComponent Functions Begin

    UFUNCTION()
    virtual void OnSaveComboEvent();

    UFUNCTION()
    virtual void OnResetComboEvent();

    // UAlphabetAttackComponent Functions End

public:
    // UAlphabetAttackComponent Dispatchers Begin

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Alphabet | AttackComponent")
    FOnSaveCombo OnSaveCombo;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Alphabet | AttackComponent")
    FOnResetCombo OnResetCombo;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Alphabet | AttackComponent")
    FDamageStart OnDamageStart;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Alphabet | AttackComponent")
    FDamageEnd OnDamageEnd;

    // UAlphabetAttackComponent Dispatchers End

public:
    // UAlphabetAttackComponent Variables Begin

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Alphabet | AttackComponent")
    EAttackCollisionLocation CollisionLocation;

    UPROPERTY(EditDefaultsOnly, Category = "Alphabet | AttackComponent")
    bool bIsRandomAttack = false;

    // UAlphabetAttackComponent Variables End
};
