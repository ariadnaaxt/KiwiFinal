// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/GameTypes.h"
#include "GameplayActors/InteractableActor.h"
#include "CinematicInteractableActor.generated.h"

//Delegate to be broadcasted when the cinematic is finished
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCinematicFinishedDelegate);


/**
 * 
 */
UCLASS()
class FEATHER_API ACinematicInteractableActor : public AInteractableActor
{
	GENERATED_BODY()

public:

	ACinematicInteractableActor();
	
	//Level Sequencer to reproduce when interacted
	UPROPERTY(EditAnywhere, Category = "Cinematic")
	class ULevelSequence* LevelSequence;

	//Delegate to be broadcasted when the cinematic is finished
	UPROPERTY(BlueprintAssignable, Category = "Cinematic")
	FOnCinematicFinishedDelegate OnCinematicFinishedDelegate;

	UPROPERTY(EditAnywhere, Category = "Cinematic")
	bool bShouldMovePlayerAfterCinematic = false;

	UPROPERTY(EditAnywhere, Category = "Cinematic")
	AActor* PlayerDestination;

	UPROPERTY(EditAnywhere, Category = "Cinematic")
	E_GameState GameStateAfterCinematic = E_GameState::EGS_Explore;

	UPROPERTY(EditAnywhere, Category = "Cinematic")
	bool bChangeViewModeAfterCinematic = false;

	UPROPERTY(EditAnywhere, Category = "Cinematic")
	E_ViewMode ViewModeAfterCinematic = E_ViewMode::EVM_ThirdPerson;

	UPROPERTY(EditAnywhere, Category = "Cinematic")
	bool bShouldBlockReturn = false;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* InvisibleWallComponent;
protected:
	//Function to be called when the cinematic is finished
	UFUNCTION()
	void OnCinematicFinished();

	//Override of the OnInteracted function
	virtual void OnInteracted(APawn* InstigatorPawn) override;

	UPROPERTY()
	class ULevelSequencePlayer* LevelSequencePlayer;

	UPROPERTY()
	class ALevelSequenceActor* LevelSequenceActor;
};
