// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/FeatherGameState.h"

void AFeatherGameState::SetGameState(E_GameState NewGameState)
{
	CurrentGameState = NewGameState;
	OnGameStateChange.Broadcast(NewGameState);
}
