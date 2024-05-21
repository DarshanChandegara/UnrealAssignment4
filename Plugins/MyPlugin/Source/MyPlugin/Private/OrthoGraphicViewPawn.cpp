// Fill out your copyright notice in the Description page of Project Settings.


#include "OrthoGraphicViewPawn.h"
#include <EnhancedInputComponent.h>
#include "InteractiveArchController.h"
#include <EnhancedInputSubsystems.h>

// Sets default values
AOrthoGraphicViewPawn::AOrthoGraphicViewPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	scene = CreateDefaultSubobject<USceneComponent>(TEXT("scene"));
	RootComponent = scene;

	CameraBoom = CreateAbstractDefaultSubobject<USpringArmComponent>(TEXT("boom"));
	CameraBoom->AttachToComponent(scene, FAttachmentTransformRules::KeepRelativeTransform);
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 5.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("camera"));
	Camera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform);

	movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	MoveScale = 1.f;
	LookScale = 200.f;
}


void AOrthoGraphicViewPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOrthoGraphicViewPawn::Tick(float DeltaTime)
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

// Called to bind functionality to input
void AOrthoGraphicViewPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	mapping = NewObject<UInputMappingContext>(this);

	MoveAction = NewObject<UInputAction>(this);
	MoveAction->ValueType = EInputActionValueType::Axis2D;

	MapKey(mapping , MoveAction ,EKeys::W);
	MapKey(mapping , MoveAction ,EKeys::S , true);
	MapKey(mapping , MoveAction ,EKeys::D , false , true);
	MapKey(mapping , MoveAction ,EKeys::A , true , true);

	LookAction = NewObject<UInputAction>(this);
	LookAction->ValueType = EInputActionValueType::Axis3D;

	MapKey(mapping , LookAction , EKeys::MouseX ,false , true);

	ZoomAction = NewObject<UInputAction>(this);
	ZoomAction->ValueType = EInputActionValueType::Axis1D;

	MapKey(mapping, ZoomAction, EKeys::MouseWheelAxis);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	AInteractiveArchController* MPC = Cast<AInteractiveArchController>(Controller);

	if (EIC && MPC) {
		EIC->BindAction(MoveAction , ETriggerEvent::Triggered , this , &AOrthoGraphicViewPawn::Move);
		EIC->BindAction(LookAction , ETriggerEvent::Triggered , this , &AOrthoGraphicViewPawn::Look);
		EIC->BindAction(ZoomAction , ETriggerEvent::Triggered , this , &AOrthoGraphicViewPawn::Zoom);

		ULocalPlayer* LocalPlayer = MPC->GetLocalPlayer();
		check(LocalPlayer);
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		check(Subsystem);
		Subsystem->ClearAllMappings(); 
		Subsystem->AddMappingContext(mapping, 0);
	}

}

void AOrthoGraphicViewPawn::Move(const FInputActionValue& Value)
{

	FVector myVec;
	myVec.X = Value.Get<FInputActionValue::Axis2D>().X;
	myVec.Y = Value.Get<FInputActionValue::Axis2D>().Y;

	FVector Input = GetActorRotation().RotateVector(myVec);
	Input.Z = 0; 
	AddMovementInput(Input, 1);
}

void AOrthoGraphicViewPawn::Look(const FInputActionValue& Value)
{
	FRotator Input(Value[0], Value[1], Value[2]);
	Input *= GetWorld()->GetDeltaSeconds() * 50;

	Input += GetActorRotation();

	SetActorRotation(Input);
}

void AOrthoGraphicViewPawn::Zoom(const FInputActionValue& Value)
{
	float value = Value.Get<float>();
	CameraBoom->TargetOffset.Z -= value * 25;
	CameraBoom->TargetOffset.Z = FMath::Clamp(CameraBoom->TargetOffset.Z, 200, 2000);
}
