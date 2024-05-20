// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <InputAction.h>
#include "InputMappingContext.h"
#include "WallSpline.h"
#include "WallBuilderController.generated.h"

DECLARE_DELEGATE_OneParam(FNotifyDelegate, FString message)

UCLASS()
class MYPLUGIN_API AWallBuilderController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY()
	AWallSpline* currActor;

	UPROPERTY()
	int32 currActorIndex;

public:

	UPROPERTY()
	UInputMappingContext* mapping;

	UPROPERTY()
	UInputAction* LeftClickAction;

	UPROPERTY()
	UInputAction* RightClickAction;

	UPROPERTY()
	UInputAction* UndoAction;

	UPROPERTY()
	UInputAction* LeftArrowAction;

	UPROPERTY()
	UInputAction* RightArrowAction;

	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> points;

	AWallBuilderController();

	void OnLeftClick(const FInputActionValue& value);
	void OnRightClick(const FInputActionValue& value);
	void Undo(const FInputActionValue& value);
	void LeftArrow(const FInputActionValue& value);
	void RightArrow(const FInputActionValue& value);

	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<AWallSpline*> WallSplineArray;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateWidget();

	UFUNCTION(BlueprintCallable)
	void DestroyWalls();

	UFUNCTION(BlueprintCallable)
	void DestroyCurrentWall();

	FNotifyDelegate Notification;

	UFUNCTION(BlueprintImplementableEvent)
	void Notify(const FString& message);
	
};
