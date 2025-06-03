#pragma once

#include "ECharacterType.h"
#include "FDialogConfiguration.generated.h"

USTRUCT(BlueprintType)
struct FDialogConfiguration
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	ECharacterType CharacterType = ECharacterType::KIWI;
	
	UPROPERTY(EditDefaultsOnly)
	FText Text = FText::FromString("");
	
	UPROPERTY(EditDefaultsOnly)
	float CustomTypingSpeed = 0.03;
};
