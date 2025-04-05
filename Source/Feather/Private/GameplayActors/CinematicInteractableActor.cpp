// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/CinematicInteractableActor.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Components/BoxComponent.h"
#include "FeatherCharacter/FeatherCharacter.h"
#include "Framework/FeatherGameState.h"

ACinematicInteractableActor::ACinematicInteractableActor()
{
	InvisibleWallComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InvisibleWallComponent"));
	InvisibleWallComponent->SetupAttachment(RootComponent);
	InvisibleWallComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InvisibleWallComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	InvisibleWallComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}


void ACinematicInteractableActor::OnInteracted(APawn* InstigatorPawn)
{
	if(LevelSequence)
	{
		LevelSequenceActor = GetWorld()->SpawnActor<ALevelSequenceActor>(ALevelSequenceActor::StaticClass(), GetActorLocation(), GetActorRotation());
		LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, FMovieSceneSequencePlaybackSettings(), LevelSequenceActor);
		LevelSequencePlayer->Play();
		LevelSequencePlayer->OnFinished.AddDynamic(this, &ACinematicInteractableActor::OnCinematicFinished);
		//Set new game state
		GAME_STATE->SetGameState(E_GameState::EGS_Cinematic);
	}
}

void ACinematicInteractableActor::OnCinematicFinished()
{
	OnCinematicFinishedDelegate.Broadcast();
	LevelSequencePlayer->Stop();
	LevelSequenceActor->Destroy();
	LevelSequenceActor = nullptr;
	LevelSequencePlayer = nullptr;
	
	if(bChangeViewModeAfterCinematic)
	{	
		FVector PlayerDestinationLocation = FVector::ZeroVector;
		FRotator PlayerDestinationRotation = FRotator::ZeroRotator;
		if(AFeatherCharacter* FeatherCharacter = Cast<AFeatherCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
		{
			if(!PlayerDestination)
			{
				PlayerDestinationLocation = FeatherCharacter->GetActorLocation();
				PlayerDestinationRotation = FeatherCharacter->GetActorRotation();
			}
			else
			{
				PlayerDestinationLocation = PlayerDestination->GetActorLocation();
				PlayerDestinationRotation = PlayerDestination->GetActorRotation();
			}
			FeatherCharacter->ChangeViewMode(ViewModeAfterCinematic, PlayerDestinationLocation, PlayerDestinationRotation);
		}
	}
	else if(bShouldMovePlayerAfterCinematic)
	{
		if(!PlayerDestination)
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerDestination not set in CinematicInteractableActor %s"), *GetName());
			return;
		}
		if(APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			PlayerPawn->SetActorLocation(PlayerDestination->GetActorLocation());
		}
	}

	if(bShouldBlockReturn)
	{
		InvisibleWallComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	
	GAME_STATE->SetGameState(GameStateAfterCinematic);
	DisableInteractable();
}

