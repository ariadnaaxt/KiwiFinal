// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/HarvestInteractableActor.h"

void AHarvestInteractableActor::OnInteracted(APawn* InstigatorPawn)
{
	OnObjectHarvestedDelegate.Broadcast();
	DisableInteractable();
}
