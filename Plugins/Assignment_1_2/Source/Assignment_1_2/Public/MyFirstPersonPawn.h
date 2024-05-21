// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 
#include "MyBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "MyFirstPersonPawn.generated.h"

UCLASS()
class ASSIGNMENT_1_2_API AMyFirstPersonPawn : public AMyBaseCharacter
{
	GENERATED_BODY()

public:
	AMyFirstPersonPawn();

	void MapKey(UInputMappingContext* mappingContext, UInputAction* action, FKey key, bool bNegate, bool bSwizzel, EInputAxisSwizzle swizzleOrder);

	void Move(const struct FInputActionValue& action);

	void Rotate(const struct FInputActionValue& action);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



public:
	UPROPERTY(EditAnywhere)
	USpringArmComponent* springArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* camera;

	UPROPERTY(EditAnywhere)
	class UFloatingPawnMovement* movement;

	UPROPERTY(EditAnywhere)
	float MoveScale;

	UPROPERTY(EditAnywhere)
	float RotateScale;

	UPROPERTY()
	UInputMappingContext* pawnMapping;

	UPROPERTY()
	UInputAction* moveAction;

	UPROPERTY()
	UInputAction* rotateAction;

	UPROPERTY()
	UInputAction* jumpAction; 

};
