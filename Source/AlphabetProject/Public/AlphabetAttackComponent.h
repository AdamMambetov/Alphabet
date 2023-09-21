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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAbilityUse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityUpdate, float, NewAbilityValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShotFireUpdate, float, NewShotFireValue);

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

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // UActorComponent End

public:
    // UAttackComponent Begin

    virtual void Attack() override;
    virtual void AttackById(const FGameplayTag AttackId) override;

protected:
    virtual FGameplayTag GetNextAttackId() override;

    // UAttackComponent End

public:
    // UAlphabetAttackComponent Functions Begin

    UFUNCTION(BlueprintCallable, Category = "Alphabet | AttackComponent")
    void UseAbility();

    UFUNCTION(BlueprintCallable, Category = "Alphabet | AttackComponent")
    void SpawnShotFire();

protected:
    void UpdateAbilityValue();
    void UpdateShotFireValue();

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

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Alphabet | AttackComponent")
    FAbilityUpdate OnAbilityUpdate;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Alphabet | AttackComponent")
    FAbilityUse OnAbilityUse;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Alphabet | AttackComponent")
    FShotFireUpdate OnShotFireUpdate;

    // UAlphabetAttackComponent Dispatchers End

public:
    // UAlphabetAttackComponent Variables Begin

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Alphabet | AttackComponent")
    EAttackCollisionLocation CollisionLocation;

    UPROPERTY(EditDefaultsOnly, Category = "Alphabet | AttackComponent")
    bool bIsRandomAttack = false;

    UPROPERTY(BlueprintReadOnly, Category = "Alphabet | AttackComponent")
    bool bUseAbility = false;

    UPROPERTY(BlueprintReadOnly, Category = "Alphabet | AttackComponent")
    float AbilityValue = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Alphabet | AttackComponent")
    float AbilityRechargeTime = 3.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Alphabet | AttackComponent")
    float AbilityUseTime = 3.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Alphabet | AttackComponent")
    TSubclassOf<AActor> ShotFireClass;

    UPROPERTY(BlueprintReadOnly, Category = "Alphabet | AttackComponent")
    float ShotFireValue = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Alphabet | AttackComponent")
    float ShotFireRechargeTime = 3.f;

    // UAlphabetAttackComponent Variables End
};
