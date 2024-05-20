// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include <GameFramework/FloatingPawnMovement.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "IsometricViewPawn.generated.h"

UCLASS()
class MYPLUGIN_API AIsometricViewPawn : public APawn
{
	GENERATED_BODY()

public:
	AIsometricViewPawn();

	void Zoom(const FInputActionValue& Value);
	void RotateQ(const FInputActionValue& Value);
	void RotateE(const FInputActionValue& Value);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	USceneComponent* scene;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY()
	UInputMappingContext* mapping;

	UPROPERTY()
	UInputAction* RotateActionQ;

	UPROPERTY()
	UInputAction* RotateActionE;

	UPROPERTY()
	UInputAction* ZoomAction;

	UPROPERTY()
	UFloatingPawnMovement* movement;

	int ZoomScale;

	float TargetYaw;

};
