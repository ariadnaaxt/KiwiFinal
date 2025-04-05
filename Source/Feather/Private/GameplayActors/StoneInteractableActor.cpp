// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/StoneInteractableActor.h"
#include "FeatherCharacter/FeatherCharacter.h"
#include "Kismet/GameplayStatics.h"

void AStoneInteractableActor::OnInteracted(APawn* InstigatorPawn)
{
	AFeatherCharacter* FeatherCharacter = Cast<AFeatherCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//Attach the stone to the player
	if(FeatherCharacter && FeatherCharacter->CanGrabInteractable())
	{
		FeatherCharacter->AddStone(this);
		MeshComponent->UpdateComponentToWorld();
	}
}

void AStoneInteractableActor::OnReleased()
{
	//Actor simulate physics
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);	
	MeshComponent->OnComponentHit.AddUniqueDynamic(this, &AStoneInteractableActor::OnHit);
}

void AStoneInteractableActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->OnComponentHit.RemoveDynamic(this, &AStoneInteractableActor::OnHit);
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}
