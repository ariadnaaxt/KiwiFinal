// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/InteractableComponent.h"

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractableComponent::OnInteract(APawn* InstigatorPawn)
{
	OnInteractDelegate.Broadcast(InstigatorPawn);
}
