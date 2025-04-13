// Copyright Epic Games, Inc. All Rights Reserved.

#include "FeatherCharacter/FeatherCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ActorComponents/InteractableComponent.h"
#include "Components/SphereComponent.h"
#include "GameplayActors/InteractableActor.h"
#include "GameplayActors/StoneCollectorInteractableActor.h"
#include "GameplayActors/StoneInteractableActor.h"
#include "Framework/FeatherGameState.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AFeatherCharacter

AFeatherCharacter::AFeatherCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create interaction sphere
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AFeatherCharacter::AddStone(AStoneInteractableActor* NewStone)
{
	CurrentInteractableActor = NewStone;
	NewStone->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "PickSocket");
}

bool AFeatherCharacter::CanGrabInteractable() const
{
	return CurrentInteractableActor == nullptr;
}

void AFeatherCharacter::PutStoneInCollector(AStoneCollectorInteractableActor* Collector)
{
	CurrentInteractableActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	AStoneInteractableActor* StoneActor = Cast<AStoneInteractableActor>(CurrentInteractableActor);
	Collector->OnStoneCollected(StoneActor);
	CurrentInteractableActor = nullptr;
}

AInteractableActor* AFeatherCharacter::GetCurrentInteractableActor() const
{
	return CurrentInteractableActor;
}

void AFeatherCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	InitializeCharacter();
	GAME_STATE->SetGameState(E_GameState::EGS_Explore);
	GAME_STATE->OnGameStateChange.AddDynamic(this, &AFeatherCharacter::OnGameStateChanged);
}

void AFeatherCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(CameraState == E_CameraState::ECS_Moving)
	{
		MoveCameraToNewRotationSmoothed(CurrentNewRotation, DeltaSeconds);
	}
}

void AFeatherCharacter::InitializeCharacter() const
{
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFeatherCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	SetupThirdPersonInput(PlayerInputComponent);
}

void AFeatherCharacter::MoveThirdPerson(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AFeatherCharacter::MoveSideScroller(const FInputActionValue& Value)
{
	// Obtén la rotación del Controller (que controla la cámara)
	FRotator ControlRotation = GetControlRotation();
    
	// Queremos ignorar el "Pitch" para mantener el movimiento en un plano horizontal
	ControlRotation.Pitch = 0.0f;
    
	// Obtener el vector hacia adelante (Forward) en relación a la cámara
	FVector ForwardVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);
    
	// Obtener el vector a la derecha (Right) en relación a la cámara
	FVector RightVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);
    
	// Obtener el input de movimiento (X para adelante/atrás, Y para izquierda/derecha)
	FVector2D MovementVector = Value.Get<FVector2D>();
    
	// Aplicar movimiento hacia adelante/atrás y izquierda/derecha en base a los vectores calculados
	AddMovementInput(ForwardVector, MovementVector.X);  // Movimiento adelante/atrás
	AddMovementInput(RightVector, MovementVector.Y);    // Movimiento izquierda/derecha
}

void AFeatherCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFeatherCharacter::StartToCrouch()
{
	AFeatherCharacter::Crouch();
}

void AFeatherCharacter::StopToCrouch()
{
	AFeatherCharacter::UnCrouch();
	AFeatherCharacter::UnCrouch();
}

void AFeatherCharacter::StartToSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AFeatherCharacter::StopToSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

void AFeatherCharacter::Interact()
{
	TArray<AActor*> OverlappingActors;
	InteractionSphere->GetOverlappingActors(OverlappingActors, AInteractableActor::StaticClass());
	// get closest actor
	const AInteractableActor* ClosestActor = nullptr;
	float MinDistance = TNumericLimits<float>::Max();
	
	for (AActor* Actor : OverlappingActors)
	{
		if (const AInteractableActor* InteractableActor = Cast<AInteractableActor>(Actor))
		{
			if(InteractableActor == CurrentInteractableActor) continue;
			const float Distance = FVector::DistSquared(InteractableActor->GetActorLocation(), GetActorLocation());
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestActor = InteractableActor;
			}
		}
	}
	if (ClosestActor)
	{
		if(UInteractableComponent* InteractableComponent = Cast<UInteractableComponent>(ClosestActor->FindComponentByClass(UInteractableComponent::StaticClass())))
		{
			InteractableComponent->OnInteract(this);
		}
	}
	else
	{
		if(CurrentInteractableActor)
		{
			//Detach the stone from the player
			CurrentInteractableActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CurrentInteractableActor->OnReleased();
			CurrentInteractableActor = nullptr;
		}
	}
}

void AFeatherCharacter::ChangeViewMode(E_ViewMode NewViewMode, FVector NewLocation, FRotator NewRotation)
{
	if(CurrentViewMode == NewViewMode) return;
	
	switch (NewViewMode)
	{
		case E_ViewMode::EVM_ThirdPerson:
			ChangeToThirdPerson();
			break;
		case E_ViewMode::EVM_SideScroller:
			ChangeToSideScroller(NewLocation, NewRotation);
			break;
	}
}

void AFeatherCharacter::ChangeToThirdPerson()
{
	InputComponent->ClearActionBindings();	
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bUsePawnControlRotation = false;
	CurrentViewMode = E_ViewMode::EVM_ThirdPerson;	
	CurrentNewRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	CameraState = E_CameraState::ECS_Moving;
	GetCharacterMovement()->bConstrainToPlane = false;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

void AFeatherCharacter::ChangeToSideScroller(FVector3d NewLocation, FRotator NewRotation)
{
	InputComponent->ClearActionBindings();
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bUsePawnControlRotation = false;
	CurrentViewMode = E_ViewMode::EVM_SideScroller;
	FRotator CameraAngle = FRotator(-10.0f, NewRotation.Yaw - 90.0f, 0);
	CurrentNewRotation = CameraAngle;
	CameraState = E_CameraState::ECS_Moving;
	//FVector RightVector = FRotationMatrix(NewRotation).GetUnitAxis(EAxis::Y);
	//GetCharacterMovement()->SetPlaneConstraintNormal(RightVector);
	//GetCharacterMovement()->bConstrainToPlane = true;
	//GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	//SetActorLocation(NewLocation);
	Controller->SetControlRotation(NewRotation);
	SideScrollerDirection = NewRotation.Vector();
}

void AFeatherCharacter::MoveCameraToNewRotationSmoothed(FRotator NewRotation, float DeltaTime)
{
	FRotator DesiredCameraAngle = NewRotation;
	FRotator CurrentCameraAngle = CameraBoom->GetComponentRotation();

	FRotator SmoothedCameraAngle = FMath::RInterpTo(CurrentCameraAngle, DesiredCameraAngle, DeltaTime, CameraInterpSpeed);
	CameraBoom->SetWorldRotation(SmoothedCameraAngle);
	
	if (SmoothedCameraAngle.Equals(DesiredCameraAngle, 1.f))
	{
		CameraState = E_CameraState::ECS_Idle;
		if(CurrentViewMode == E_ViewMode::EVM_SideScroller)
		{
			CameraBoom->SetUsingAbsoluteRotation(true);
			CameraBoom->bDoCollisionTest = false;
			CameraBoom->bUsePawnControlRotation = false;
			SetupSideScrollerInput(InputComponent);
			
		}
		else
		{
			CameraBoom->SetUsingAbsoluteRotation(false);
			CameraBoom->bDoCollisionTest = true;
			CameraBoom->bUsePawnControlRotation = true;
			SetupThirdPersonInput(InputComponent);
		}
	}
}

void AFeatherCharacter::SetupThirdPersonInput(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveTPAction, ETriggerEvent::Triggered, this, &AFeatherCharacter::MoveThirdPerson);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFeatherCharacter::Look);

		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AFeatherCharacter::StartToCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AFeatherCharacter::StopToCrouch);

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AFeatherCharacter::StartToSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AFeatherCharacter::StopToSprint);

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AFeatherCharacter::Interact);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AFeatherCharacter::SetupSideScrollerInput(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveSSAction, ETriggerEvent::Triggered, this, &AFeatherCharacter::MoveSideScroller);

		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AFeatherCharacter::StartToCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AFeatherCharacter::StopToCrouch);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AFeatherCharacter::OnGameStateChanged(E_GameState NewGameState)
{
	switch (NewGameState)
	{
		case E_GameState::EGS_Explore:			
			GetMesh()->SetVisibility(true);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Explore"));
			break;
		case E_GameState::EGS_Pursuit:
			GetMesh()->SetVisibility(true);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Pursuit"));
			break;
		case E_GameState::EGS_Cinematic:
			//Hide mesh
			GetMesh()->SetVisibility(false);
			break;
		
	}
}
