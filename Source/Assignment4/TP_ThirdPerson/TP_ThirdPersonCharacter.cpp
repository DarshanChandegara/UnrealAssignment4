// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_ThirdPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/FloatingPawnMovement.h"


ATP_ThirdPersonCharacter::ATP_ThirdPersonCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("movement"));

	MoveScale = 1.f;
	RotateScale = 50.f;

	PawnAttributeAsset = LoadObject<UMyPawnAttributesDataAsset>(nullptr, TEXT("/Script/Assignment_1_2.MyPawnAttributesDataAsset'/Game/Assignment2/TPP_PawnAttributeDataAsset.TPP_PawnAttributeDataAsset'"));
}

void ATP_ThirdPersonCharacter::MapKey(UInputMappingContext* mappingContext, UInputAction* action, FKey key, bool bNegate = false, bool bSwizzel = false, EInputAxisSwizzle swizzleOrder = EInputAxisSwizzle::YXZ) {

	FEnhancedActionKeyMapping& mapping = mappingContext->MapKey(action, key);
	UObject* outer = mappingContext->GetOuter();

	if (bNegate) {
		UInputModifierNegate* negate = NewObject<UInputModifierNegate>(outer);
		mapping.Modifiers.Add(negate);
	}

	if (bSwizzel) {
		UInputModifierSwizzleAxis* swizzle = NewObject<UInputModifierSwizzleAxis>(outer);
		swizzle->Order = swizzleOrder;
		mapping.Modifiers.Add(swizzle);
	}
}

void ATP_ThirdPersonCharacter::BeginPlay()
{

	// Call the base class  
	Super::BeginPlay();

}

void ATP_ThirdPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		pawnMapping = NewObject<UInputMappingContext>(this);
		moveAction = NewObject<UInputAction>(this);
		moveAction->ValueType = EInputActionValueType::Axis3D;


		MapKey(pawnMapping, moveAction, EKeys::W);
		MapKey(pawnMapping, moveAction, EKeys::S, true);
		MapKey(pawnMapping, moveAction, EKeys::A, true, true);
		MapKey(pawnMapping, moveAction, EKeys::D, false, true);


		rotateAction = NewObject<UInputAction>(this);
		rotateAction->ValueType = EInputActionValueType::Axis3D;
		MapKey(pawnMapping, rotateAction, EKeys::MouseY, true);
		MapKey(pawnMapping, rotateAction, EKeys::MouseX, false, true);

		JumpAction = NewObject<UInputAction>(this);
		JumpAction->ValueType = EInputActionValueType::Boolean;
		MapKey(pawnMapping, JumpAction, EKeys::SpaceBar);

		UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		APlayerController* MPC = Cast<APlayerController>(Controller);
		check(EIC && MPC);

		EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EIC->BindAction(moveAction, ETriggerEvent::Triggered, this, &ATP_ThirdPersonCharacter::Move);

		//Looking
		EIC->BindAction(rotateAction, ETriggerEvent::Triggered, this, &ATP_ThirdPersonCharacter::Look);

		ULocalPlayer* LocalPlayer = MPC->GetLocalPlayer();
		check(LocalPlayer);
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		check(Subsystem);
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(pawnMapping, 0);
	}

}

void ATP_ThirdPersonCharacter::Move(const FInputActionValue& action)
{
	FVector2D MovementVector = action.Get<FVector2D>();
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
	}
}


void ATP_ThirdPersonCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (GetController()) {
		AddControllerYawInput(LookAxisVector.Y);
		AddControllerPitchInput(LookAxisVector.X);
	}
}




