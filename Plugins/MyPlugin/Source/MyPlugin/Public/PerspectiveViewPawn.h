// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <InputMappingContext.h>
#include <Components/CapsuleComponent.h>
#include "PerspectiveViewPawn.generated.h"

UCLASS()
class MYPLUGIN_API APerspectiveViewPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APerspectiveViewPawn();

	void Move(const struct FInputActionValue& action);

	void Rotate(const struct FInputActionValue& action);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* capsule;

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

};
