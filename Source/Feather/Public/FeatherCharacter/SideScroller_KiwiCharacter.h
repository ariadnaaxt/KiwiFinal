// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/GameTypes.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "SideScroller_KiwiCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


UCLASS(config = Game)
class ASideScroller_KiwiCharacter: public ACharacter
{
	GENERATED_BODY()

#pragma region Camera
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
#pragma endregion Camera

#pragma region Input
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveTPAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveSSAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
#pragma endregion Input

	UPROPERTY()
	class AInteractableActor* CurrentInteractableActor;

public:
	ASideScroller_KiwiCharacter();

protected:

#pragma region Input Methods
	/** Called for movement input */
	void MoveSideScroller(const FInputActionValue& Value);

	/** Called for crouching input */
	void StartToCrouch();
	void StopToCrouch();

	/** Called for sprinting input */
	void StartToSprint();
	void StopToSprint();

#pragma endregion Input Methods

#pragma region Speed variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Speed, meta = (AllowPrivateAccess = "true"))
	float NormalSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Speed, meta = (AllowPrivateAccess = "true"))
	float SprintSpeed;
#pragma endregion Speed variables


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

	void SetupSideScrollerInput(class UInputComponent* PlayerInputComponent);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

