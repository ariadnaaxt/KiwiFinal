// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KillZActor.generated.h"

UCLASS()
class FEATHER_API AKillZActor : public AActor
{
	GENERATED_BODY()
		
public:
	
	AKillZActor();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "KillZ", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* KillZone;

	UFUNCTION()
	void OnKillZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
