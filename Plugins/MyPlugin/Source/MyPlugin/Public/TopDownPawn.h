// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <GameFramework/FloatingPawnMovement.h>
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputModifiers.h"
#include "TopDownPawn.generated.h"

UCLASS()
class MYPLUGIN_API ATopDownPawn : public APawn
{
	GENERATED_BODY()

public:
	ATopDownPawn();

	void move(const FInputActionValue& value);
	void Zoom(const FInputActionValue& value);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(EditAnywhere)
	USceneComponent* scene;

	UPROPERTY(EditAnywhere)
	UCameraComponent* camera;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* springArm;

	UPROPERTY(EditAnywhere)
	UFloatingPawnMovement* movement;

	UPROPERTY()
	UInputMappingContext* mapping;

	UPROPERTY()
	UInputAction* MoveAction;

	UPROPERTY()
	UInputAction* ZoomAction;

	UPROPERTY()
	float moveScale;

	UPROPERTY()
	float ZoomScale;

};
