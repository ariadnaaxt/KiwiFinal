// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/ChangeGameStateActor.h"
#include "Components/BoxComponent.h"
#include "FeatherCharacter/FeatherCharacter.h"

AChangeGameStateActor::AChangeGameStateActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	OverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComponent"));
	OverlapComponent->SetupAttachment(RootComponent);
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AChangeGameStateActor::BeginPlay()
{
	Super::BeginPlay();
	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &AChangeGameStateActor::OnBoxCollisionBeginOverlap);
}

void AChangeGameStateActor::OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AFeatherCharacter* FeatherCharacter = Cast<AFeatherCharacter>(OtherActor))
	{
		GAME_STATE->SetGameState(GameState);
		OverlapComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OverlapComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		OverlapComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AChangeGameStateActor::OnBoxCollisionBeginOverlap);
	}
}

