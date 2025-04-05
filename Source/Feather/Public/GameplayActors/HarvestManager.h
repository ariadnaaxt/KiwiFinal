// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HarvestManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHarvestedCompleteSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHarvestedUpdateSignature);

UCLASS()
class FEATHER_API AHarvestManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHarvestManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitializeActor();

	UFUNCTION()
	void OnObjectHarvested();

	UPROPERTY(BlueprintAssignable)
	FOnHarvestedCompleteSignature OnHarvestedCompleteDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnHarvestedUpdateSignature OnHarvestedUpdateDelegate;

	UPROPERTY(BlueprintReadOnly)
	int32 HarvestedObjects = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 TotalObjects = 0;
};
