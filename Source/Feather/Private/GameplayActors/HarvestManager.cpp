// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/HarvestManager.h"

#include "GameplayActors/HarvestInteractableActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHarvestManager::AHarvestManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AHarvestManager::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		InitializeActor();
	});
	
	
}

void AHarvestManager::InitializeActor()
{
	//Get all harvest interactable actors
	TArray<AActor*> HarvestInteractableActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHarvestInteractableActor::StaticClass(), HarvestInteractableActors);

	TotalObjects = HarvestInteractableActors.Num();

	if(TotalObjects == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No Harvest Interactable Actors found in the level"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Harvest Interactable Actors found in the level"));
		Destroy();
	}

	//Bind to the OnObjectHarvested event
	for(AActor* Actor : HarvestInteractableActors)	
	{
		if(AHarvestInteractableActor* HarvestInteractableActor = Cast<AHarvestInteractableActor>(Actor))
		{
			HarvestInteractableActor->OnObjectHarvestedDelegate.AddUniqueDynamic(this, &AHarvestManager::OnObjectHarvested);
		}
	}
}

void AHarvestManager::OnObjectHarvested()
{
	HarvestedObjects += 1;
	OnHarvestedUpdateDelegate.Broadcast();
	if(HarvestedObjects == TotalObjects)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("All objects harvested"));
		OnHarvestedCompleteDelegate.Broadcast();
	}
}

