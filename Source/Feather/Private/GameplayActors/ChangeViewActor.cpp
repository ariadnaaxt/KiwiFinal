// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/ChangeViewActor.h"
#include "Components/ArrowComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "FeatherCharacter/FeatherCharacter.h"

AChangeViewActor::AChangeViewActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);
	
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);

	OverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComponent"));
	OverlapComponent->SetupAttachment(RootComponent);
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	InvisibleWallComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InvisibleWallComponent"));
	InvisibleWallComponent->SetupAttachment(RootComponent);
	InvisibleWallComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InvisibleWallComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	InvisibleWallComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
	BillboardComponent->SetupAttachment(RootComponent);
}

void AChangeViewActor::BeginPlay()
{
	Super::BeginPlay();
	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &AChangeViewActor::OnBoxCollisionBeginOverlap);
}

void AChangeViewActor::OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AFeatherCharacter* FeatherCharacter = Cast<AFeatherCharacter>(OtherActor))
	{
		FeatherCharacter->ChangeViewMode(ViewMode, BillboardComponent->GetComponentLocation(), GetActorRotation());
		InvisibleWallComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		OverlapComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OverlapComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		OverlapComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AChangeViewActor::OnBoxCollisionBeginOverlap);
	}
}

