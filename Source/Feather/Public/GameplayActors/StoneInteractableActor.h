// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayActors/InteractableActor.h"
#include "StoneInteractableActor.generated.h"

/**
 * 
 */
UCLASS()
class FEATHER_API AStoneInteractableActor : public AInteractableActor
{
	GENERATED_BODY()

protected:
	virtual void OnInteracted(APawn* InstigatorPawn) override;

	virtual void OnReleased() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	
};
