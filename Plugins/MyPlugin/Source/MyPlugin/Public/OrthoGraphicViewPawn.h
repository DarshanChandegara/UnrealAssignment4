// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <GameFramework/SpringArmComponent.h>
#include "InputMappingContext.h"
#include "InputAction.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/FloatingPawnMovement.h>
#include "OrthoGraphicViewPawn.generated.h"

UCLASS()
class MYPLUGIN_API AOrthoGraphicViewPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY()
	UInputMappingContext* mapping;

	UPROPERTY()
	UInputAction* LookAction;

	UPROPERTY()
	UInputAction* MoveAction;

	UPROPERTY()
	UInputAction* ZoomAction;

	UPROPERTY()
	UFloatingPawnMovement* movement;

	int MoveScale;
	int LookScale;

public:
	AOrthoGraphicViewPawn();

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Zoom(const FInputActionValue& Value);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MapKey(UInputMappingContext* mapping, UInputAction* action, FKey key, bool bNegate = false, bool bSwizzle = false, EInputAxisSwizzle swizzleOrder = EInputAxisSwizzle::YXZ);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;
};
