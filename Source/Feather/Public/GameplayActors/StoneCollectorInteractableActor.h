// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayActors/InteractableActor.h"
#include "StoneCollectorInteractableActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollectionCompletedDelegate);
/**
 * 
 */
UCLASS()
class FEATHER_API AStoneCollectorInteractableActor : public AInteractableActor
{
	GENERATED_BODY()

	//Positions to set stones
	UPROPERTY(EditAnywhere, Category = "Stones")
	TArray<FVector> StonePositions;

	//Amount of stones to collect
	UPROPERTY(EditAnywhere, Category = "Stones")
	int32 StonesToCollect;

	UPROPERTY()
	int32 StonesCollected;

protected:

	virtual void OnInteracted(APawn* InstigatorPawn) override;

public:

	virtual void OnStoneCollected(class AStoneInteractableActor* StoneActor);

	UPROPERTY(BlueprintAssignable)
	FOnCollectionCompletedDelegate OnCollectionCompletedDelegate;
	
};
