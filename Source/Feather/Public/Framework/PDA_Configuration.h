#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FDialogConfiguration.h"
#include "PDA_Configuration.generated.h"

USTRUCT(BlueprintType)
struct FDialogConfigWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FDialogConfiguration> Dialogs;
};

UCLASS()
class FEATHER_API UPDA_Configuration : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	TArray<FDialogConfiguration> GetDialogFromId(const FName& DialogId) const;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, FDialogConfigWrapper> Dialogs;
};
