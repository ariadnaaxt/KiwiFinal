// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/StoneCollectorInteractableActor.h"

#include "FeatherCharacter/FeatherCharacter.h"
#include "GameplayActors/StoneInteractableActor.h"

void AStoneCollectorInteractableActor::OnInteracted(APawn* InstigatorPawn)
{
	if(AFeatherCharacter* FeatherCharacter = Cast<AFeatherCharacter>(InstigatorPawn))
	{
		if(FeatherCharacter->GetCurrentInteractableActor())
		{
			if(AStoneInteractableActor* StoneInteractableActor = Cast<AStoneInteractableActor>(FeatherCharacter->GetCurrentInteractableActor()))
			{
				FeatherCharacter->PutStoneInCollector(this);
			}
		}
	}
}

void AStoneCollectorInteractableActor::OnStoneCollected(AStoneInteractableActor* StoneActor)
{
	//Put the stone in the collector attachment point
	StoneActor->AttachToComponent(MeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, NAME_None);
	StoneActor->SetActorRelativeLocation(StonePositions[StonesCollected]);
	StonesCollected++;
	StoneActor->DisableInteractable();
	//Check if all stones are collected
	if(StonesCollected == StonesToCollect)
	{
		OnCollectionCompletedDelegate.Broadcast();
		DisableInteractable();
	}
}
