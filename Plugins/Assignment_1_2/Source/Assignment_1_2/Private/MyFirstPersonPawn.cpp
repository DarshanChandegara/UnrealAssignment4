// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFirstPersonPawn.h"
#include "../Public/MyPlayerController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AMyFirstPersonPawn::AMyFirstPersonPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetRootComponent());
	//springArm->bUsePawnControlRotation = true; 

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm, USpringArmComponent::SocketName);
	//camera->bUsePawnControlRotation = true;  

	movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVE"));
	bUseControllerRotationPitch = false; 
	bUseControllerRotationYaw = false;

	MoveScale = 1.f;
	RotateScale = 100.f;

	PawnAttributeAsset = LoadObject<UMyPawnAttributesDataAsset>(nullptr, TEXT("/Script/Assignment_1_2.MyPawnAttributesDataAsset'/Game/Assignment2/FPP_PawnAttributeDataAsset.FPP_PawnAttributeDataAsset'"));
}

static void MapKey(UInputMappingContext* mappingContext, UInputAction* action, FKey key, bool bNegate = false, bool bSwizzel = false, EInputAxisSwizzle swizzleOrder = EInputAxisSwizzle::YXZ) {

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

void AMyFirstPersonPawn::BeginPlay()
{
	Super::BeginPlay();	
}

void AMyFirstPersonPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyFirstPersonPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	pawnMapping = NewObject<UInputMappingContext>(this);
	moveAction = NewObject<UInputAction>(this);
	moveAction->ValueType = EInputActionValueType::Axis3D;
	//pawnMapping->MapKey(moveAction , EKeys::W);

	//FEnhancedActionKeyMapping& mapping = pawnMapping->MapKey(moveAction, EKeys::S);
	//UInputModifierNegate* negate = NewObject<UInputModifierNegate>(this);
	//mapping.Modifiers.Add(negate);

	MapKey(pawnMapping, moveAction, EKeys::W);
	MapKey(pawnMapping, moveAction, EKeys::S, true);
	MapKey(pawnMapping, moveAction, EKeys::A, true, true);
	MapKey(pawnMapping, moveAction, EKeys::D, false, true);

	// TO move in Z direction 
	// 
	//MapKey(pawnMapping, moveAction, EKeys::SpaceBar , false , true , EInputAxisSwizzle::ZYX); 
	//MapKey(pawnMapping, moveAction, EKeys::LeftShift , true , true , EInputAxisSwizzle::ZYX);  



	rotateAction = NewObject<UInputAction>(this);
	rotateAction->ValueType = EInputActionValueType::Axis3D;
	MapKey(pawnMapping, rotateAction, EKeys::MouseY);
	MapKey(pawnMapping, rotateAction, EKeys::MouseX, false ,true); 
  
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	APlayerController* MPC = Cast<APlayerController>(Controller);
	check(EIC && MPC);
	EIC->BindAction(moveAction, ETriggerEvent::Triggered, this, &AMyFirstPersonPawn::Move);
	EIC->BindAction(rotateAction, ETriggerEvent::Triggered, this, &AMyFirstPersonPawn::Rotate);

	jumpAction = NewObject<UInputAction>(this);
	jumpAction->ValueType = EInputActionValueType::Boolean;
	pawnMapping->MapKey(jumpAction , EKeys::SpaceBar); 
	EIC->BindAction(jumpAction, ETriggerEvent::Completed, this, &ACharacter::Jump);

	ULocalPlayer* LocalPlayer = MPC->GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(pawnMapping, 0);
}

void AMyFirstPersonPawn::Move(const FInputActionValue& action)
{
	FVector myVec;
	myVec.X = action.Get<FInputActionValue::Axis2D>().X;
	myVec.Y = action.Get<FInputActionValue::Axis2D>().Y;

	FVector Input = GetActorRotation().RotateVector(myVec);
	Input.Z = 0;  // This will stop pawn to go in up and down direction 
	//FVector Input = action.Get<FInputActionValue::Axis3D>(); // Here we use only X and Y but we pass this to vector that's why we create 3D
	//AddMovementInput(myVec, MoveScale);  
	AddMovementInput(Input, MoveScale);
}

void AMyFirstPersonPawn::Rotate(const FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(-1,1,FColor::Green , "this");
	FRotator Input(Value[0], Value[1], Value[2]);
	Input *= GetWorld()->GetDeltaSeconds() * RotateScale;

	Input += GetActorRotation();
	Input.Pitch = FMath::ClampAngle(Input.Pitch, -89.9f, 89.9f);
	Input.Roll = 0;

	SetActorRotation(Input);

}

