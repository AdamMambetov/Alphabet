//

#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "CharacterInfo.generated.h"

class UTexture2D;
class AAlphabetCharacter;

USTRUCT(BlueprintType)
struct FCharacterInfo : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    FCharacterInfo() {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterInfo", meta = (Categories = "Character"))
    FGameplayTag Id;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterInfo")
    FText Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterInfo")
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterInfo")
    TSubclassOf<AAlphabetCharacter> Class;
};
