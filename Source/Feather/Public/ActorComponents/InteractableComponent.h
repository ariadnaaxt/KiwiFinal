// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractDelegate, APawn*, InstigatorPawn);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FEATHER_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractableComponent();

	virtual void OnInteract(APawn* InstigatorPawn);

	UPROPERTY()
	FOnInteractDelegate OnInteractDelegate;
};
