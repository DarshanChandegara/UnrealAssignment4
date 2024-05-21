// Fill out your copyright notice in the Description page of Project Settings.


#include "PerspectiveViewPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractiveArchController.h"


APerspectiveViewPawn::APerspectiveViewPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = capsule;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetRootComponent());
	springArm->bEnableCameraLag = true;
	springArm->CameraLagSpeed = 5.f;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm, USpringArmComponent::SocketName);

	movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVE"));
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	MoveScale = 1.f;
	RotateScale = 100.f;

}



void APerspectiveViewPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void APerspectiveViewPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

static void MapKey(UInputMappingContext* mapping, UInputAction* action, FKey key, bool bNegate = false, bool bSwizzle = false, EInputAxisSwizzle swizzleOrder = EInputAxisSwizzle::YXZ) {

	FEnhancedActionKeyMapping& map = mapping->MapKey(action, key);
	UObject* outer = mapping->GetOuter();

	if (bNegate) {
		UInputModifierNegate* negate = NewObject<UInputModifierNegate>(outer);
		map.Modifiers.Add(negate);
	}

	if (bSwizzle) {
		UInputModifierSwizzleAxis* swizzle = NewObject<UInputModifierSwizzleAxis>(outer);
		swizzle->Order = swizzleOrder;
		map.Modifiers.Add(swizzle);
	}
}

void APerspectiveViewPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
	MapKey(pawnMapping, moveAction, EKeys::Q , false , true , EInputAxisSwizzle::ZYX); 
	MapKey(pawnMapping, moveAction, EKeys::E , true , true , EInputAxisSwizzle::ZYX);  

	// TO move in Z direction 
	// 
	//MapKey(pawnMapping, moveAction, EKeys::SpaceBar , false , true , EInputAxisSwizzle::ZYX); 
	//MapKey(pawnMapping, moveAction, EKeys::LeftShift , true , true , EInputAxisSwizzle::ZYX);  



	rotateAction = NewObject<UInputAction>(this);
	rotateAction->ValueType = EInputActionValueType::Axis3D;
	MapKey(pawnMapping, rotateAction, EKeys::MouseY);
	MapKey(pawnMapping, rotateAction, EKeys::MouseX, false, true);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	AInteractiveArchController* MPC = Cast<AInteractiveArchController>(Controller);
	check(EIC && MPC);
	EIC->BindAction(moveAction, ETriggerEvent::Triggered, this, &APerspectiveViewPawn::Move);
	EIC->BindAction(rotateAction, ETriggerEvent::Triggered, this, &APerspectiveViewPawn::Rotate);

	ULocalPlayer* LocalPlayer = MPC->GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(pawnMapping, 0);
}

void APerspectiveViewPawn::Move(const FInputActionValue& action)
{
	FVector myVec = action.Get<FInputActionValue::Axis3D>();

	FVector Input = GetActorRotation().RotateVector(myVec);

	AddMovementInput(Input, MoveScale);
}

void APerspectiveViewPawn::Rotate(const FInputActionValue& action)
{
	FRotator Input(action[0], action[1], action[2]);
	Input *= GetWorld()->GetDeltaSeconds() * RotateScale;

	Input += GetActorRotation();
	Input.Pitch = FMath::ClampAngle(Input.Pitch, -89.9f, 89.9f);
	Input.Roll = 0;

	SetActorRotation(Input);
}