// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/KillZActor.h"

#include "Components/BoxComponent.h"
#include "Interfaces/ZKillable.h"

AKillZActor::AKillZActor()
{
	PrimaryActorTick.bCanEverTick = false;

	KillZone = CreateDefaultSubobject<UBoxComponent>(TEXT("KillZone"));
	SetRootComponent(KillZone);
}

void AKillZActor::BeginPlay()
{
	Super::BeginPlay();

	KillZone->OnComponentBeginOverlap.AddDynamic(this, &AKillZActor::OnKillZoneOverlap);
}

void AKillZActor::OnKillZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(IZKillable* Killable = Cast<IZKillable>(OtherActor))
	{
		Killable->OnKilled();
	}
}

