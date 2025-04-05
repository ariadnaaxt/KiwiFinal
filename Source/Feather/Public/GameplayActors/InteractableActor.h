// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ZKillable.h"
#include "InteractableActor.generated.h"

UCLASS()
class FEATHER_API AInteractableActor : public AActor, public IZKillable
{
	GENERATED_BODY()
	
public:	
	AInteractableActor();
	
	UFUNCTION()
	virtual void OnReleased();

	UFUNCTION()
	virtual void DisableInteractable();

protected:
	
	virtual void BeginPlay() override;

	FTransform OriginalTransform;

	FVector OriginalScale;
	
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Interactable")
	class UInteractableComponent* InteractableComponent;

	UFUNCTION()
	virtual void OnInteracted(APawn* InstigatorPawn);

	virtual void OnKilled() override;

	
};
