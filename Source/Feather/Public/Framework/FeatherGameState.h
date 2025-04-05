// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Framework/GameTypes.h"
#include "FeatherGameState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChange, E_GameState, NewGameState);

#define GAME_STATE (GetWorld()->GetGameState<AFeatherGameState>())

UCLASS()
class FEATHER_API AFeatherGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void SetGameState(E_GameState NewGameState);

	UFUNCTION()
	E_GameState GetGameState() const { return CurrentGameState; }

	UPROPERTY()
	FOnGameStateChange OnGameStateChange;

protected:
	
	UPROPERTY()
	E_GameState CurrentGameState;
	
};
