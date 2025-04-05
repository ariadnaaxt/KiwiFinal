// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameTypes.h"
#include "GameFramework/PlayerController.h"
#include "FeatherPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPauseStateChangeDelegate, bool, NewPauseState);

/**
 * 
 */
UCLASS()
class FEATHER_API AFeatherPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnGameStateChange(E_GameState NewGameState);

	virtual void SetupInputComponent() override;

	UFUNCTION()
	void ChangePauseState();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* ControllerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PauseAction;

public:
	FOnPauseStateChangeDelegate OnPauseStateChange;
	
};
