// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/InteractableActor.h"

#include "ActorComponents/InteractableComponent.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));

}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableComponent->OnInteractDelegate.AddDynamic(this, &AInteractableActor::OnInteracted);

	OriginalTransform = GetActorTransform();

	OriginalScale = MeshComponent->GetRelativeTransform().GetScale3D();
}

void AInteractableActor::OnInteracted(APawn* InstigatorPawn)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Interacted"));
}

void AInteractableActor::OnKilled()
{
	SetActorTransform(OriginalTransform + FTransform(FVector(0, 0, 25)));
	MeshComponent->SetRelativeScale3D(OriginalScale);
}

void AInteractableActor::OnReleased()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Released"));
}

void AInteractableActor::DisableInteractable()
{
	InteractableComponent->OnInteractDelegate.RemoveAll(this);
	InteractableComponent->DestroyComponent();
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}

