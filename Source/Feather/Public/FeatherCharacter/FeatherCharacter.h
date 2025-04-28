// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Framework/GameTypes.h"
#include "FeatherCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AFeatherCharacter : public ACharacter
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
	AFeatherCharacter();

	UFUNCTION()
	void AddStone(class AStoneInteractableActor* NewStone);

	UFUNCTION()
	bool CanGrabInteractable() const;

	UFUNCTION()
	void PutStoneInCollector(class AStoneCollectorInteractableActor* Collector);

	UFUNCTION()
	class AInteractableActor* GetCurrentInteractableActor() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom")
	void OnInteractionPressed();
	
protected:

#pragma region Input Methods
	/** Called for movement input */
	void MoveThirdPerson(const FInputActionValue& Value);
	void MoveSideScroller(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for crouching input */
	void StartToCrouch();
	void StopToCrouch();

	/** Called for sprinting input */
	void StartToSprint();
	void StopToSprint();

	/** Called for interacting input */
	void Interact();
#pragma endregion Input Methods

#pragma region Speed variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed, meta = (AllowPrivateAccess = "true"))
	float NormalSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed, meta = (AllowPrivateAccess = "true"))
	float SprintSpeed;
#pragma endregion Speed variables

public:
	UFUNCTION(BlueprintCallable)
	void ChangeViewMode(E_ViewMode NewViewMode, FVector NewLocation, FRotator NewRotation);
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

	// To initialize the character
	void InitializeCharacter() const;

	// To Change the view mode
	void ChangeToThirdPerson();
	void ChangeToSideScroller(FVector3d NewLocation = FVector3d::ZeroVector, FRotator NewRotation = FRotator::ZeroRotator);
	E_ViewMode CurrentViewMode = E_ViewMode::EVM_ThirdPerson;
	// To Set Camera State
	void MoveCameraToNewRotationSmoothed(FRotator NewRotation, float DeltaTime);
	E_CameraState CameraState = E_CameraState::ECS_Idle;
	FRotator CurrentNewRotation;
	FVector3d SideScrollerDirection;
	
	UPROPERTY(EditDefaultsOnly, Category = Camera, meta = (AllowPrivate))
	float CameraInterpSpeed = 5.f;

	void SetupThirdPersonInput(class UInputComponent* PlayerInputComponent);
	void SetupSideScrollerInput(class UInputComponent* PlayerInputComponent);

	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivate))
	class USphereComponent* InteractionSphere;

	UFUNCTION()
	void OnGameStateChanged(E_GameState NewGameState);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

