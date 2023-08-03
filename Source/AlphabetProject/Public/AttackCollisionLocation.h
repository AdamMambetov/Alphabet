//

#pragma once

#include "AttackCollisionLocation.generated.h"

UENUM(BlueprintType)
enum class EAttackCollisionLocation : uint8
{
    Left UMETA(DisplayName = "Left"),
    Right UMETA(DisplayName = "Right"),
};
