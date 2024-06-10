// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

ACustomPlayerController::ACustomPlayerController()
{
	bShowMouseCursor = true;
}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	auto SpawnActor = GetWorld()->SpawnActor<ASelectionArea>(SelectionAreaClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);

	if (AreaActor = Cast<ASelectionArea>(SpawnActor); AreaActor) {
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "A:SHI Fds  z");
	}

	auto SpawnActor1 = GetWorld()->SpawnActor<AMeshGenerator>(MeshGeneratorClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);
	if (MeshGeneratorActor = Cast<AMeshGenerator>(SpawnActor1); MeshGeneratorActor) {

	}
}

void ACustomPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isMoving) {
		if (FVector StartLocation, WorldDirection; DeprojectMousePositionToWorld(StartLocation, WorldDirection))
		{
			FVector EndLocation = StartLocation + WorldDirection * 100000;

			FCollisionQueryParams Params;
			Params.bTraceComplex = true;
			Params.AddIgnoredActor(AreaActor);

			if (FHitResult HitResult; GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation,
				ECC_Visibility, Params))
			{
				FVector Location = HitResult.Location;
				Location.Z = 0;
				FVector PrevLocation = FVector(0, 0, AreaActor->GetActorLocation().Z);
				AreaActor->SetActorLocation(Location + PrevLocation);
			}
		}
	}
}

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	Mapping = NewObject<UInputMappingContext>(this);

	LeftClickAction = NewObject<UInputAction>(this);
	LeftClickAction->ValueType = EInputActionValueType::Boolean;

	Mapping->MapKey(LeftClickAction, EKeys::LeftMouseButton);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);
	if (EIC) {

		EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &ACustomPlayerController::LeftClick);

		ULocalPlayer* LocalPlayer = GetLocalPlayer();
		check(LocalPlayer);

		if (LocalPlayer) {

			UEnhancedInputLocalPlayerSubsystem* Subsystem =
				LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			check(Subsystem);

			if (Subsystem) {

				Subsystem->AddMappingContext(Mapping, 0);
			}
		}

	}
}

void ACustomPlayerController::GenerateAreaShape(int index)
{

	if (AreaActor) {
		FVector PrevLocation = AreaActor->GetActorLocation();
		PrevLocation.Z = 0;

		switch (index)
		{
		case 0:
			AreaActor->GenerateBox(FVector(Lentgh , Width ,Height));
			AreaActor->SetActorLocation(FVector(0, 0, Height/2) + PrevLocation);
			break;

		case 1:
			AreaActor->GenerateSphere(Radius);
			AreaActor->SetActorLocation(FVector(0, 0, Radius) + PrevLocation);
			break;

		default:
			break;
		}
	}
}

void ACustomPlayerController::HandleButtonClick(int index)
{
	MeshGeneratorActor->Location = AreaActor->GetActorLocation();

	if (index == 0) {
		MeshGeneratorActor->AreaType = EAreaType::Box;
		MeshGeneratorActor->Dimension = FVector(Lentgh, Width, Height) / 2;
	}
	else if(index == 1)
	{
		MeshGeneratorActor->AreaType = EAreaType::Sphere;
		MeshGeneratorActor->Dimension = FVector(Radius);
	}

	MeshGeneratorActor->NumberOfInstances = NumberOfInstances;

	MeshGeneratorActor->ScatterObjects();
}

void ACustomPlayerController::LeftClick()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "click");

	isMoving = !isMoving;
	bShowMouseCursor = true;
}

