// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveArchController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Kismet/KismetMaterialLibrary.h>

void AInteractiveArchController::BeginPlay()
{
	Super::BeginPlay();

	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "Press P to Change The View");

	if (SelectionWidgetClass) {
		SelectionWidget = CreateWidget<USelectionWidget>(this, SelectionWidgetClass);
	}

	if (MyUserWidgetClass) {
		MyUserWidget = CreateWidget<UMyUserWidget>(this, MyUserWidgetClass);
	}


	SelectionWidget->MeshSelectionScrollBox->OnMeshSelectEvent.BindUObject(this, &AInteractiveArchController::HandleMeshSelection);
	SelectionWidget->MeshSelectionScrollBox->OnMaterialSelectEvent.BindUObject(this, &AInteractiveArchController::HandleMaterialSelection);
	SelectionWidget->MeshSelectionScrollBox->OnTextureSelectEvent.BindUObject(this, &AInteractiveArchController::HandleTextureSelection);
	SelectionWidget->MaterialSelectionScrollBox->OnMeshSelectEvent.BindUObject(this, &AInteractiveArchController::HandleMeshSelection);
	SelectionWidget->MaterialSelectionScrollBox->OnMaterialSelectEvent.BindUObject(this, &AInteractiveArchController::HandleMaterialSelection);
	SelectionWidget->MaterialSelectionScrollBox->OnTextureSelectEvent.BindUObject(this, &AInteractiveArchController::HandleTextureSelection);
	SelectionWidget->TextureSelectionScrollBox->OnMeshSelectEvent.BindUObject(this, &AInteractiveArchController::HandleMeshSelection);
	SelectionWidget->TextureSelectionScrollBox->OnMaterialSelectEvent.BindUObject(this, &AInteractiveArchController::HandleMaterialSelection);
	SelectionWidget->TextureSelectionScrollBox->OnTextureSelectEvent.BindUObject(this, &AInteractiveArchController::HandleTextureSelection);

	AWallSpline* splineBuilder = NewObject<AWallSpline>(this);
	WallSplineArray.Add(splineBuilder);
	currActor = WallSplineArray[WallSplineArray.Num() - 1];
	currActorIndex = 0;
	CreateMyWidget();

	Notification.BindUFunction(this, "Notify");

	SpawnAndPossess();
}

void AInteractiveArchController::MapKey(UInputMappingContext* mapping, UInputAction* action, FKey key, bool bNegate, bool bSwizzle, EInputAxisSwizzle swizzleOrder) {

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

AInteractiveArchController::AInteractiveArchController()
{
	bShowMouseCursor = true;
	bOnSelect = false;
	count = 1;
}

void AInteractiveArchController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Mesh Generation Mappings 
	MeshGenerationMapping = NewObject<UInputMappingContext>(this);
	WallBuilderMapping = NewObject<UInputMappingContext>(this);

	LeftClickAction = NewObject<UInputAction>(this);
	LeftClickAction->ValueType = EInputActionValueType::Boolean;

	MapKey(MeshGenerationMapping ,LeftClickAction, EKeys::LeftMouseButton);

	ToggleAction = NewObject<UInputAction>(this);
	ToggleAction->ValueType = EInputActionValueType::Boolean;

	MapKey(MeshGenerationMapping , ToggleAction, EKeys::Tab);


	// Wall Builder  Mappings 

	LeftMouseClickAction = NewObject<UInputAction>(this);
	LeftMouseClickAction->ValueType = EInputActionValueType::Boolean;

	MapKey(WallBuilderMapping, LeftMouseClickAction, EKeys::LeftMouseButton);

	RightMouseClickAction = NewObject<UInputAction>(this);
	RightMouseClickAction->ValueType = EInputActionValueType::Boolean;

	MapKey(WallBuilderMapping, RightMouseClickAction, EKeys::RightMouseButton);

	UndoAction = NewObject<UInputAction>(this);
	UndoAction->ValueType = EInputActionValueType::Boolean;

	MapKey(WallBuilderMapping, UndoAction, EKeys::Z);

	LeftArrowAction = NewObject<UInputAction>(this);
	LeftArrowAction->ValueType = EInputActionValueType::Boolean;

	MapKey(WallBuilderMapping, LeftArrowAction, EKeys::J
	);

	RightArrowAction = NewObject<UInputAction>(this);
	RightArrowAction->ValueType = EInputActionValueType::Boolean;

	MapKey(WallBuilderMapping, RightArrowAction, EKeys::L);



	// ViewPawn Possess Mappings 

	PossessMapping = NewObject<UInputMappingContext>();
	PossessAction = NewObject<UInputAction>();
	PossessAction->ValueType = EInputActionValueType::Boolean;

	MapKey(PossessMapping ,PossessAction, EKeys::P);



	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	if (EIC) {
		EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AInteractiveArchController::OnLeftClickMeshFunction);
		EIC->BindAction(ToggleAction, ETriggerEvent::Completed, this, &AInteractiveArchController::Toggle);

		EIC->BindAction(LeftMouseClickAction, ETriggerEvent::Completed, this, &AInteractiveArchController::OnLeftClickWallFunction);
		EIC->BindAction(RightMouseClickAction, ETriggerEvent::Completed, this, &AInteractiveArchController::OnRightClick);
		EIC->BindAction(UndoAction, ETriggerEvent::Completed, this, &AInteractiveArchController::Undo);
		EIC->BindAction(LeftArrowAction, ETriggerEvent::Completed, this, &AInteractiveArchController::LeftArrow);
		EIC->BindAction(RightArrowAction, ETriggerEvent::Completed, this, &AInteractiveArchController::RightArrow);

		EIC->BindAction(PossessAction, ETriggerEvent::Completed, this, &AInteractiveArchController::SpawnAndPossess);


		UEnhancedInputLocalPlayerSubsystem* Subsytem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

		if (Subsytem) {
			Subsytem->AddMappingContext(MeshGenerationMapping, 0);
			Subsytem->AddMappingContext(PossessMapping, 0);
		}
	}
}

void AInteractiveArchController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->AddMappingContext(PossessMapping, 0);

	if (count % 2 == 1) {
		Subsystem->AddMappingContext(MeshGenerationMapping, 0);
	}
	else {
		Subsystem->AddMappingContext(WallBuilderMapping, 0);
	}
}

void AInteractiveArchController::OnLeftClickMeshFunction()
{

	FHitResult HitResult;

	FVector MouseLocation, MouseDirection;
	DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
	FVector EndLocation = MouseLocation + MouseDirection * 7000;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, MouseLocation, EndLocation, ECC_Visibility);

	if (bHit) {

		HitLocation = HitResult.Location;
		//if (!SelectionWidget->IsInViewport()) {
			SelectionWidget->AddToViewport();
			SelectionWidget->MeshSelectionScrollBox->SetVisibility(ESlateVisibility::Visible);
		//}

		if (HitResult.GetActor()) {
			AArchMeshActor* Actor = Cast<AArchMeshActor>(HitResult.GetActor());
			if (Actor) {
				CurrentActor = Actor;
				bOnSelect = true;
				SelectionWidget->MeshSelectionScrollBox->SetVisibility(ESlateVisibility::Visible);
				SelectionWidget->MaterialSelectionScrollBox->SetVisibility(ESlateVisibility::Visible);
				SelectionWidget->TextureSelectionScrollBox->SetVisibility(ESlateVisibility::Visible);
				DynamicMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, CurrentActor->GetStaticMeshComponent()->GetMaterial(0));
				GetPawn()->SetActorLocation(HitLocation + FVector(0,0,50));
			}
			else {
				bOnSelect = false;
			}
		}

	}
}

void AInteractiveArchController::OnLeftClickWallFunction()
{
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

void AInteractiveArchController::Toggle()
{
	if (SelectionWidget->MeshSelectionScrollBox->GetVisibility() == ESlateVisibility::Visible) {
		SelectionWidget->MeshSelectionScrollBox->SetVisibility(ESlateVisibility::Collapsed);
		SelectionWidget->MaterialSelectionScrollBox->SetVisibility(ESlateVisibility::Collapsed);
		SelectionWidget->TextureSelectionScrollBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (SelectionWidget->MeshSelectionScrollBox->GetVisibility() == ESlateVisibility::Collapsed) {
		SelectionWidget->MeshSelectionScrollBox->SetVisibility(ESlateVisibility::Visible);
	}
}

void AInteractiveArchController::ActorSpawn(const FMeshData& MeshData)
{
	if (MeshData.Mesh) {

		if (PreviousHitLocation == HitLocation) CurrentActor->Destroy();
		if (CurrentActor && bOnSelect)
			CurrentActor->Destroy();
		FVector Min = MeshData.Mesh->GetBoundingBox().Min;
		FVector Max = MeshData.Mesh->GetBoundingBox().Max;

		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		HitLocation.Z = 0;
		AArchMeshActor* MyActor = GetWorld()->SpawnActor<AArchMeshActor>(AArchMeshActor::StaticClass(), HitLocation + FVector(0, 0, -Min.Z), FRotator::ZeroRotator, params);

		if (MyActor) {
			MyActor->SetMobility(EComponentMobility::Movable);
			MyActor->GetStaticMeshComponent()->SetStaticMesh(MeshData.Mesh);

			CurrentActor = MyActor;
			DynamicMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, CurrentActor->GetStaticMeshComponent()->GetMaterial(0));
			GetPawn()->SetActorLocation(HitLocation + FVector(0, 0, 50));
		}
		PreviousHitLocation = HitLocation;
	}
}

void AInteractiveArchController::HandleMeshSelection(const FMeshData& MeshData)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Mesh");
	ActorSpawn(MeshData);
	SelectionWidget->MaterialSelectionScrollBox->SetVisibility(ESlateVisibility::Visible);
	SelectionWidget->TextureSelectionScrollBox->SetVisibility(ESlateVisibility::Visible);
}

void AInteractiveArchController::HandleMaterialSelection(const FMaterialData& MaterialData)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Material");
	CurrentActor->GetStaticMeshComponent()->SetMaterial(0, MaterialData.Material);

	DynamicMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, CurrentActor->GetStaticMeshComponent()->GetMaterial(0));

}

void AInteractiveArchController::HandleTextureSelection(const FTextureData& TextureData)
{
	if (DynamicMaterial) {

		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "2");

		DynamicMaterial->SetTextureParameterValue("Texture", TextureData.Texture); // Set texture parameter in material
		CurrentActor->GetStaticMeshComponent()->SetMaterial(0, DynamicMaterial);
	}
}

void AInteractiveArchController::Switching()
{
	count++;
	if (count % 2 == 1) {

		MyUserWidget->RemoveFromViewport();
		SelectionWidget->AddToViewport();

		UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

		if (EIC) {

			UEnhancedInputLocalPlayerSubsystem* Subsytem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

			if (Subsytem) {
				Subsytem->RemoveMappingContext(WallBuilderMapping);
				Subsytem->AddMappingContext(MeshGenerationMapping, 0);
			}
		}
	}
	else {

		SelectionWidget->RemoveFromViewport();
		MyUserWidget->AddToViewport();

		UEnhancedInputComponent* EIC = Cast< UEnhancedInputComponent>(InputComponent);
		if (EIC) {

			ULocalPlayer* player = GetLocalPlayer();
			if (player) {
				UEnhancedInputLocalPlayerSubsystem* subsystem = player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

				if (subsystem) {
					subsystem->RemoveMappingContext(MeshGenerationMapping);
					subsystem->AddMappingContext(WallBuilderMapping, 0);
				}
			}
		}
	}
}

void AInteractiveArchController::SpawnAndPossess()
{
	Table = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Assignment4/DataTable/ViewPawnTable_.ViewPawnTable_'"));
	if (Table)
	{
		TArray<FName> RowNames = Table->GetRowNames();

		FName RandomRowName = RowNames[Index % RowNames.Num()];
		Index++;

		FViewPawnTable* RowData = Table->FindRow<FViewPawnTable>(RandomRowName, TEXT(""));

		if (RowData) {
			auto oldType = ViewPawnType;
			ViewPawnType = RowData->ViewPawnType;
			UClass* characterToSpawn = RowData->ViewPawn;
			if (characterToSpawn) {
				FActorSpawnParameters params;
				params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			
				FRotator rotate = FRotator::ZeroRotator;

				AActor* spawnActor = GetWorld()->SpawnActor<AActor>(characterToSpawn, HitLocation + FVector(-50,0,50), rotate, params);
				if (spawnActor) {
					APawn* reference = Cast<APawn>(spawnActor);
					if (reference) {

						UEnhancedInputLocalPlayerSubsystem* Subsystem =
							ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());


						auto old = currentPawn;
						Possess(reference);

						if (old)
							old->Destroy();
						currentPawn = reference;
					}
				}
			}

		}
	}
}

void AInteractiveArchController::LeftClick(const FInputActionValue& value)
{
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

void AInteractiveArchController::OnRightClick(const FInputActionValue& value)
{
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

void AInteractiveArchController::Undo(const FInputActionValue& value)
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
}

void AInteractiveArchController::LeftArrow(const FInputActionValue& value)
{
	if (currActorIndex != 0) {
		currActorIndex--;
		currActor = WallSplineArray[currActorIndex];
	}
	else {
		Notification.ExecuteIfBound("There is nothing to backward");
	}
}

void AInteractiveArchController::RightArrow(const FInputActionValue& value)
{
	if (currActorIndex != WallSplineArray.Num() - 1) {
		currActorIndex++;
		currActor = WallSplineArray[currActorIndex];
	}
	else {
		Notification.ExecuteIfBound("There is nothing to forward");
	}
}

void AInteractiveArchController::DestroyWalls()
{
	for (int i = 0; i < WallSplineArray.Num(); i++) {
		WallSplineArray[i]->ClearPoints();
		WallSplineArray[i]->DestroyWalls();
	}

	WallSplineArray.Empty();

	AWallSpline* splineBuilder = NewObject<AWallSpline>(this);
	WallSplineArray.Add(splineBuilder);
	currActor = WallSplineArray[WallSplineArray.Num() - 1];
}

void AInteractiveArchController::DestroyCurrentWall()
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
