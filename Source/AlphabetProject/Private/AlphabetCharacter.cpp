// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AlphabetCharacter.h"
#include "AlphabetStatsComponent.h"
#include "AlphabetAttackComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

AAlphabetCharacter::AAlphabetCharacter()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate when the controller rotates.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;            // Face in the direction we are moving..
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
    GetCharacterMovement()->GravityScale = 2.f;
    GetCharacterMovement()->AirControl = 0.80f;
    GetCharacterMovement()->JumpZVelocity = 1000.f;
    GetCharacterMovement()->GroundFriction = 3.f;
    GetCharacterMovement()->MaxWalkSpeed = 600.f;
    GetCharacterMovement()->MaxFlySpeed = 600.f;

    GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

    // Configure stats
    StatsComponent = CreateDefaultSubobject<UAlphabetStatsComponent>(TEXT("Stats Component"));

    // Configure attack
    AttackComponent = CreateDefaultSubobject<UAlphabetAttackComponent>(TEXT("Attack Component"));
    AttackComponent->CollisionLocation = EAttackCollisionLocation::Right;

    // Configure Weapon collision
    WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Collision"));
    WeaponCollision->SetupAttachment(GetMesh(), TEXT("RightAttackSocket"));
    WeaponCollision->InitBoxExtent(FVector(50.f));
    WeaponCollision->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

    // Configure Health Widget
    HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Widget"));
    HealthWidget->SetupAttachment(GetRootComponent());
    HealthWidget->SetWidgetSpace(EWidgetSpace::World);
    HealthWidget->SetDrawSize(FVector2D(100.f, 10.f));

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAlphabetCharacter::BeginPlay()
{
    Super::BeginPlay();

    StatsComponent->OnStatReachedZero.AddDynamic(this, &AAlphabetCharacter::OnStatReachedZero);

    WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AAlphabetCharacter::OnWeaponCollisionBeginOverlap);

    AttackComponent->OnDamageStart.AddDynamic(this, &AAlphabetCharacter::OnDamageStart);
    AttackComponent->OnDamageEnd.AddDynamic(this, &AAlphabetCharacter::OnDamageEnd);

    InitHealthWidgetBlueprint();
}

void AAlphabetCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (FMath::IsNearlyZero(GetCharacterMovement()->Velocity.Y / GetCharacterMovement()->GetMaxSpeed()))
    {
        if (!FMath::IsNearlyZero(Direction) && !GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
        {
            auto NewCollisionLocation =
                FMath::IsNegativeFloat(Direction) ? EAttackCollisionLocation::Left : EAttackCollisionLocation::Right;
            FName WeaponLocationSocket = FMath::IsNegativeFloat(Direction) ? TEXT("LeftAttackSocket") : TEXT("RightAttackSocket");

            AttackComponent->CollisionLocation = NewCollisionLocation;
            WeaponCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponLocationSocket);
        }
    }
    else
    {
        Direction = GetCharacterMovement()->Velocity.Y / GetCharacterMovement()->GetMaxSpeed() * -1.f;
    }

    if (IsValid(HealthWidget))
    {
        FVector L_Location = GetActorLocation() + FVector(0.f, 0.f, 80.f);
        FRotator L_Rotation = UKismetMathLibrary::FindLookAtRotation(
            HealthWidget->GetComponentLocation(), UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation());

        HealthWidget->SetWorldLocationAndRotation(L_Location, L_Rotation);
    }
}

void AAlphabetCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AAlphabetCharacter::OnJump);
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AAlphabetCharacter::OnStopJumping);
    PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AAlphabetCharacter::OnAttack);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AAlphabetCharacter::OnMoveRight);
}

void AAlphabetCharacter::OnJump()
{
    if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) return;
    Jump();
    OnJumpBlueprint();
}

void AAlphabetCharacter::OnStopJumping()
{
    StopJumping();
    OnStopJumpingBlueprint();
}

void AAlphabetCharacter::OnMoveRight(float AxisValue)
{
    const FVector WorldDirection = FVector(0.f, -1.f, 0.f);
    AddMovementInput(WorldDirection, AxisValue);

    if (!FMath::IsNearlyZero(AxisValue) && !GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
    {
        auto NewCollisionLocation = FMath::IsNegativeFloat(AxisValue) ? EAttackCollisionLocation::Left : EAttackCollisionLocation::Right;
        FName WeaponLocationSocket = FMath::IsNegativeFloat(AxisValue) ? TEXT("LeftAttackSocket") : TEXT("RightAttackSocket");

        AttackComponent->CollisionLocation = NewCollisionLocation;
        WeaponCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponLocationSocket);
    }

    OnMoveRightBlueprint(AxisValue);
}

void AAlphabetCharacter::OnAttack()
{
    if (GetCharacterMovement()->IsFalling()) return;
    AttackComponent->Attack();
    OnAttackBlueprint();
}

void AAlphabetCharacter::OnStatReachedZero(FStatInfo StatInfo)
{
    OnStatReachedZeroBlueprint(StatInfo);
    if (StatInfo.Id == UDefaults::MakeTag(TEXT("Stat.Health")))
    {
        Destroy();
    }
}

void AAlphabetCharacter::OnWeaponCollisionBeginOverlap( //
    UPrimitiveComponent* OverlappedComponent,           //
    AActor* OtherActor,                                 //
    UPrimitiveComponent* OtherComp,                     //
    int32 OtherBodyIndex,                               //
    bool bFromSweep,                                    //
    const FHitResult& SweepResult)                      //
{
    
    if (OtherActor == this || (OtherActor->ActorHasTag(TEXT("Player")) == ActorHasTag(TEXT("Player")))) return;
    OtherActor->TakeDamage(AttackComponent->GetCurrentAttackInfo().Damage, FDamageEvent(), GetController(), this);
    WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAlphabetCharacter::OnDamageStart()
{
    WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AAlphabetCharacter::OnDamageEnd()
{
    WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
