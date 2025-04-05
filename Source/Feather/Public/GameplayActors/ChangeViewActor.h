// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Framework/GameTypes.h"
#include "ChangeViewActor.generated.h"

UCLASS()
class FEATHER_API AChangeViewActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AChangeViewActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* OverlapComponent;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* InvisibleWallComponent;

	UPROPERTY(EditAnywhere, Category = "Direction")
	class UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, Category = "Location")
	class UBillboardComponent* BillboardComponent;

	UPROPERTY(EditAnywhere, Category = "Setup")
	E_ViewMode ViewMode;

	UFUNCTION()
	void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
