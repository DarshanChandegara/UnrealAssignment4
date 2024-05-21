// Fill out your copyright notice in the Description page of Project Settings.


#include "WallBuilderController.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

AWallBuilderController::AWallBuilderController()
{
	bShowMouseCursor = true;
}

void AWallBuilderController::BeginPlay()
{
	Super::BeginPlay();

	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Magenta, "Press Left Click To Generate Walls"); 
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Magenta, "Press Right Click To Generate New Wall");
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Magenta, "Press Z To Undo The Last Wall Segment Of Current Walls");

	AWallSpline* splineBuilder = NewObject<AWallSpline>(this);
	WallSplineArray.Add(splineBuilder);
	currActor = WallSplineArray[WallSplineArray.Num() - 1];
	currActorIndex = 0;
	CreateWidget();

	Notification.BindUFunction(this, "Notify");
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


void AWallBuilderController::OnLeftClick(const FInputActionValue& value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, "Hello");

	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (HitResult.bBlockingHit)
	{
		FVector ClickLocation = HitResult.Location;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Click Location: %s"), *ClickLocation.ToString())); 
		if (currActor) {
			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, "Inside");
			if (currActor->GetSplinePoints() == 0) {
				Notification.ExecuteIfBound("Wall Construction Has Started");
			}
			currActor->AddPoint(ClickLocation);
			currActor->GenerateWalls();
		}

	}

}

void AWallBuilderController::OnRightClick(const FInputActionValue& value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Right Click");

	if (currActor->GetSplinePoints() >= 2) {
		AWallSpline* wallSpline = NewObject<AWallSpline>(this);
		WallSplineArray.Add(wallSpline);
		currActor = WallSplineArray[WallSplineArray.Num() - 1];
		currActorIndex++;
		Notification.ExecuteIfBound("Wall Construction Has Been Finished");
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "You have to have at least 1 wall to weitch to new wall ");
	}

}

void AWallBuilderController::Undo(const FInputActionValue& value)
{
	currActor->RemoveLastPoint();
	currActor->GenerateWalls();

	if (currActor->GetSplinePoints() <= 1) {
		if (currActorIndex != 0) {
			WallSplineArray.RemoveAt(currActorIndex);
			currActorIndex--;
			currActor = WallSplineArray[currActorIndex];
		}
		else if (currActorIndex == 0 && currActorIndex != WallSplineArray.Num() - 1) {
			if (currActorIndex < WallSplineArray.Num()) {
				WallSplineArray.RemoveAt(currActorIndex);
			}
			currActor = WallSplineArray[currActorIndex];
		}
		else if (WallSplineArray.Num() == 0) {
			AWallSpline* SplineActor = NewObject<AWallSpline>(this);
			WallSplineArray.Add(SplineActor);
			currActor = SplineActor;
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, "Undo");
}

void AWallBuilderController::LeftArrow(const FInputActionValue& value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "You have Clicked Left ");
	if (currActorIndex != 0) {
		currActorIndex--;
		currActor = WallSplineArray[currActorIndex];
	}
	else {
		Notification.ExecuteIfBound("There is nothing to backward");
	}
}

void AWallBuilderController::RightArrow(const FInputActionValue& value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, "You have Clicked Right ");
	if (currActorIndex != WallSplineArray.Num() - 1) {
		currActorIndex++;
		currActor = WallSplineArray[currActorIndex];
	}
	else {
		Notification.ExecuteIfBound("There is nothing to forward");
	}
}

void AWallBuilderController::SetupInputComponent()
{
	Super::SetupInputComponent();
	mapping = NewObject<UInputMappingContext>(this);

	LeftClickAction = NewObject<UInputAction>(this);
	LeftClickAction->ValueType = EInputActionValueType::Boolean;

	MapKey(mapping, LeftClickAction, EKeys::LeftMouseButton);

	RightClickAction = NewObject<UInputAction>(this);
	RightClickAction->ValueType = EInputActionValueType::Boolean;

	MapKey(mapping, RightClickAction, EKeys::RightMouseButton);

	UndoAction = NewObject<UInputAction>(this);
	UndoAction->ValueType = EInputActionValueType::Boolean;

	MapKey(mapping, UndoAction, EKeys::Z);

	LeftArrowAction = NewObject<UInputAction>(this);
	LeftArrowAction->ValueType = EInputActionValueType::Boolean;

	MapKey(mapping, LeftArrowAction, EKeys::Left);

	RightArrowAction = NewObject<UInputAction>(this);
	RightArrowAction->ValueType = EInputActionValueType::Boolean;

	MapKey(mapping, RightArrowAction, EKeys::Right);


	UEnhancedInputComponent* EIC = Cast< UEnhancedInputComponent>(InputComponent);
	if (EIC) {
		EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AWallBuilderController::OnLeftClick);
		EIC->BindAction(RightClickAction, ETriggerEvent::Completed, this, &AWallBuilderController::OnRightClick);
		EIC->BindAction(UndoAction, ETriggerEvent::Completed, this, &AWallBuilderController::Undo);
		EIC->BindAction(LeftArrowAction, ETriggerEvent::Completed, this, &AWallBuilderController::LeftArrow);
		EIC->BindAction(RightArrowAction, ETriggerEvent::Completed, this, &AWallBuilderController::RightArrow);

		ULocalPlayer* player = GetLocalPlayer();
		if (player) {
			UEnhancedInputLocalPlayerSubsystem* subsystem = player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

			if (subsystem) {
				subsystem->AddMappingContext(mapping, 0);
			}
		}
	}
}

void AWallBuilderController::DestroyWalls()
{
	for (int i = 0; i < WallSplineArray.Num(); i++) {
		WallSplineArray[i]->ClearPoints();
		WallSplineArray[i]->DestroyWalls();
	}

	WallSplineArray.Empty();

	AWallSpline* splineBuilder = NewObject<AWallSpline>(this);
	WallSplineArray.Add(splineBuilder);
	currActor = WallSplineArray[WallSplineArray.Num() - 1];
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Destroy Walls");
}

void AWallBuilderController::DestroyCurrentWall()
{
	currActor->ClearPoints();
	currActor->DestroyWalls();
	WallSplineArray.RemoveAt(currActorIndex);
	currActorIndex--;

	if (currActorIndex == -1 && WallSplineArray.Num() == 0) {
		AWallSpline* splineBuilder = NewObject<AWallSpline>(this);
		WallSplineArray.Add(splineBuilder);
		currActor = WallSplineArray[WallSplineArray.Num() - 1];
		currActorIndex++;
	}
	else {
		if (currActorIndex == -1) {
			currActorIndex++;
		}
		currActor = WallSplineArray[currActorIndex];
	}
}

