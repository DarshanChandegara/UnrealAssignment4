// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/TopDownPawn.h"
#include "EnhancedInputComponent.h"
#include <EnhancedInputSubsystems.h>

ATopDownPawn::ATopDownPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = scene;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->TargetArmLength = -210;
	springArm->TargetOffset = FVector(-520, 0, 540);
	springArm->SetupAttachment(scene);
	float Angle = FMath::RadiansToDegrees(atan(springArm->TargetOffset.Z / springArm->TargetArmLength));

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetRelativeRotation(FRotator(Angle, 0, 0));
	camera->SetupAttachment(springArm);

	movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("movement"));

	moveScale = 1.f;
	ZoomScale = 100.f;

}

// Called when the game starts or when spawned
void ATopDownPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATopDownPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//static void MapKey(UInputMappingContext* mapping, UInputAction* action, FKey key, bool bNegate = false, bool bSwizzle = false, EInputAxisSwizzle swizzleOrder = EInputAxisSwizzle::YXZ) {
//
//	FEnhancedActionKeyMapping& map = mapping->MapKey(action, key);
//	UObject* outer = mapping->GetOuter();
//
//	if (bNegate) {
//		UInputModifierNegate* negate = NewObject<UInputModifierNegate>(outer);
//		map.Modifiers.Add(negate);
//	}
//
//	if (bSwizzle) {
//		UInputModifierSwizzleAxis* swizzle = NewObject<UInputModifierSwizzleAxis>(outer);
//		swizzle->Order = swizzleOrder;
//		map.Modifiers.Add(swizzle);
//	}
//
//}

// Called to bind functionality to input
void ATopDownPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	mapping = NewObject<UInputMappingContext>(this);
	MoveAction = NewObject<UInputAction>(this);
	MoveAction->ValueType = EInputActionValueType::Axis2D;

	MapKey(mapping, MoveAction, EKeys::W);
	MapKey(mapping, MoveAction, EKeys::S, true);
	MapKey(mapping, MoveAction, EKeys::A, true, true);
	MapKey(mapping, MoveAction, EKeys::D, false, true);

	ZoomAction = NewObject<UInputAction>(this);
	ZoomAction->ValueType = EInputActionValueType::Axis1D;

	MapKey(mapping, ZoomAction, EKeys::MouseWheelAxis);


	UEnhancedInputComponent* EIC = Cast< UEnhancedInputComponent>(PlayerInputComponent);
	if (EIC) {
		APlayerController* PC = Cast<APlayerController>(Controller);
		if (PC) {
			EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownPawn::move);
			EIC->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ATopDownPawn::Zoom);

			ULocalPlayer* player = PC->GetLocalPlayer();
			if (player) {
				UEnhancedInputLocalPlayerSubsystem* subsystem = player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

				if (subsystem) {
					//subsystem->ClearAllMappings();
					subsystem->AddMappingContext(mapping, 0);
				}
			}
		}
	}

}

void ATopDownPawn::move(const FInputActionValue& value)
{
	FVector vector;
	vector.X = value.Get<FVector2D>().X;
	vector.Y = value.Get<FVector2D>().Y;

	FVector input = GetActorRotation().RotateVector(vector);
	input.Z = 0;

	AddMovementInput(input, moveScale);
}

void ATopDownPawn::Zoom(const FInputActionValue& value)
{
	/*	float scale = value.Get<float>();
		springArm->TargetArmLength = FMath::Clamp((springArm->TargetArmLength - (scale * ZoomScale)) , 10 , INFINITY);
		FRotator rotation = springArm->GetRelativeRotation() + FRotator(scale * 0.5, 0, 0);
		rotation.Pitch = FMath::ClampAngle(rotation.Pitch, -90.f, -20.f);
		springArm->SetRelativeRotation(rotation);*/

	float Value = value.Get<float>();

	float TargetArmLength = springArm->TargetArmLength;
	float TargetOffsetZ = springArm->TargetOffset.Z;

	if (TargetArmLength >= -460) {
		springArm->TargetOffset.Z -= Value * 20;
	}
	if (TargetOffsetZ <= 1000) {
		springArm->TargetArmLength -= Value * 9.2;
	}

	springArm->TargetArmLength = FMath::Clamp(springArm->TargetArmLength, -500, -0);
	springArm->TargetOffset.Z = FMath::Clamp(springArm->TargetOffset.Z, 0, 2500);

	float Angle;
	if (springArm->TargetArmLength == 0) {
		Angle = -90;
	}
	else {
		Angle = FMath::RadiansToDegrees(atan(springArm->TargetOffset.Z / springArm->TargetArmLength));
		Angle = FMath::Clamp(Angle, -90, 0);
	}

	camera->SetRelativeRotation(FRotator(Angle, 0, 0));
}
