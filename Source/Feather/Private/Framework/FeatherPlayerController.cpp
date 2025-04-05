// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/FeatherPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Framework/FeatherGameState.h"

void AFeatherPlayerController::BeginPlay()
{
	Super::BeginPlay();
	GAME_STATE->OnGameStateChange.AddDynamic(this, &AFeatherPlayerController::OnGameStateChange);
	//Set input mode to game only
	const FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;
	
}

void AFeatherPlayerController::OnGameStateChange(E_GameState NewGameState)
{
	switch (NewGameState)
	{
		case E_GameState::EGS_Explore:
			EnableInput(this);
			break;
		
		case E_GameState::EGS_Pursuit:
			EnableInput(this);
			break;

		case E_GameState::EGS_Cinematic:
			DisableInput(this);
			break;
	}
}

void AFeatherPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(ControllerMappingContext, 0);
	}
	if(UEnhancedInputComponent* ControllerInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		ControllerInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AFeatherPlayerController::ChangePauseState);	
	}
}

void AFeatherPlayerController::ChangePauseState()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Pause"));
	SetPause(!IsPaused());
	OnPauseStateChange.Broadcast(IsPaused());
}
