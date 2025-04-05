// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayActors/InteractableActor.h"
#include "HarvestInteractableActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnObjectHarvestedSignature);

/**
 * 
 */
UCLASS()
class FEATHER_API AHarvestInteractableActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintAssignable, Category = "Harvest")
	FOnObjectHarvestedSignature OnObjectHarvestedDelegate;

protected:

	virtual void OnInteracted(APawn* InstigatorPawn) override;
	
};
