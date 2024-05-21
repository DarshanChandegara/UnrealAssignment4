// Fill out your copyright notice in the Description page of Project Settings.


#include "IsometricViewPawn.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include "InteractiveArchController.h"


AIsometricViewPawn::AIsometricViewPawn()
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

	ZoomScale = 25;

}

void AIsometricViewPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AIsometricViewPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRotation = CameraBoom->GetRelativeRotation();

	FRotator NewRotation = FRotator(CameraBoom->GetRelativeRotation().Pitch, TargetYaw, CameraBoom->GetRelativeRotation().Roll);
	CameraBoom->SetRelativeRotation(FMath::Lerp(CurrentRotation, NewRotation, DeltaTime*2));


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

void AIsometricViewPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	mapping = NewObject<UInputMappingContext>(this);

	ZoomAction = NewObject<UInputAction>(this);
	ZoomAction->ValueType = EInputActionValueType::Axis1D;

	MapKey(mapping, ZoomAction, EKeys::MouseWheelAxis);

	RotateActionQ = NewObject<UInputAction>(this);
	RotateActionQ->ValueType = EInputActionValueType::Boolean;	
	
	MapKey(mapping, RotateActionQ, EKeys::Q);

	RotateActionE = NewObject<UInputAction>(this);
	RotateActionE->ValueType = EInputActionValueType::Boolean;

	MapKey(mapping, RotateActionE, EKeys::E , true);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	AInteractiveArchController* MPC = Cast<AInteractiveArchController>(Controller);

	if (EIC && MPC) {
		EIC->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AIsometricViewPawn::Zoom);
		EIC->BindAction(RotateActionQ, ETriggerEvent::Completed, this, &AIsometricViewPawn::RotateQ);
		EIC->BindAction(RotateActionE, ETriggerEvent::Completed, this, &AIsometricViewPawn::RotateE);


		ULocalPlayer* LocalPlayer = MPC->GetLocalPlayer();
		check(LocalPlayer);
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		check(Subsystem);	
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(mapping, 0);
	}

}

void AIsometricViewPawn::Zoom(const FInputActionValue& Value)
{
	float value = Value.Get<float>();
	CameraBoom->TargetArmLength -= value * ZoomScale;
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength, 200, 2000);
}

void AIsometricViewPawn::RotateQ(const FInputActionValue& Value)
{
	TargetYaw += 45;
	TargetYaw = FMath::Wrap(TargetYaw, -180.f, 180.f);
	//if (TargetYaw > 180.0f) {
	//	TargetYaw -= 360.0f;
	//}

}

void AIsometricViewPawn::RotateE(const FInputActionValue& Value)
{
	TargetYaw += -45;
	TargetYaw = FMath::Wrap(TargetYaw, -180.f, 180.f);

	//if (TargetYaw < -180.0f) {
	//	TargetYaw += 360.0f;
	//}
}
