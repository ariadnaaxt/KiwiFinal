// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Framework/GameTypes.h"
#include "Framework/FeatherGameState.h"
#include "ChangeGameStateActor.generated.h"

UCLASS()
class FEATHER_API AChangeGameStateActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AChangeGameStateActor();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Collision")
	class UBoxComponent* OverlapComponent;

	UPROPERTY(EditAnywhere, Category = "Setup")
	E_GameState GameState;

	UFUNCTION()
	void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
