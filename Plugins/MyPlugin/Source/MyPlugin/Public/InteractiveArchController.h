// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "ArchMeshActor.h"
#include "MeshAssetManager.h"
#include "SelectionWidget.h"
#include "MyUserWidget.h"
#include "WallSpline.h"
#include "Engine/DataTable.h"
#include "OrthoGraphicViewPawn.h"
#include "PerspectiveViewPawn.h"
#include "IsometricViewPawn.h"
#include "InteractiveArchController.generated.h"

DECLARE_DELEGATE_OneParam(FNotifyDelegate, FString message)

UENUM()
enum class EViewPawnType : int8 {
	PerspectiveView ,
	OrthographicView ,
	IsometricView , 
	TopDownView
};

USTRUCT(BlueprintType)
struct FViewPawnTable : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EViewPawnType> ViewPawnType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> ViewPawn;
};

UCLASS()
class MYPLUGIN_API AInteractiveArchController : public APlayerController
{
	GENERATED_BODY()

	// Switching Variable

public:
	UPROPERTY(BlueprintReadOnly)
	int count;
	// Wall Buiilder Variables and Functions

	UPROPERTY()
	AWallSpline* currActor;

	UPROPERTY()
	int32 currActorIndex;

	UPROPERTY()
	UInputMappingContext* WallBuilderMapping;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMyUserWidget* MyUserWidget;

	UPROPERTY()
	UInputAction* LeftMouseClickAction;

	UPROPERTY()
	UInputAction* RightMouseClickAction;

	UPROPERTY()
	UInputAction* UndoAction;

	UPROPERTY()
	UInputAction* LeftArrowAction;

	UPROPERTY()
	UInputAction* RightArrowAction;

	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> points;

	void LeftClick(const FInputActionValue& value);
	void OnRightClick(const FInputActionValue& value);
	void Undo(const FInputActionValue& value);
	void LeftArrow(const FInputActionValue& value);
	void RightArrow(const FInputActionValue& value);

	UPROPERTY()
	TArray<AWallSpline*> WallSplineArray;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateMyWidget();

	UFUNCTION(BlueprintCallable)
	void DestroyWalls();

	UFUNCTION(BlueprintCallable)
	void DestroyCurrentWall();

	FNotifyDelegate Notification;

	UFUNCTION(BlueprintImplementableEvent)
	void Notify(const FString& message);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMyUserWidget> MyUserWidgetClass;





	// Mesh Genration Variables and Functions

	FVector HitLocation;

	FVector PreviousHitLocation;

	bool bOnSelect;

	UPROPERTY()
	UInputMappingContext* MeshGenerationMapping;

	UPROPERTY()
	UInputAction* LeftClickAction;

	UPROPERTY()
	UInputAction* ToggleAction;

	UPROPERTY()
	USelectionWidget* SelectionWidget;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USelectionWidget> SelectionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMeshAssetManager* MeshAssetManager;

	UPROPERTY()
	AArchMeshActor* CurrentActor;


	void OnLeftClickMeshFunction();

	void OnLeftClickWallFunction();

	void Toggle();

	UFUNCTION()
	void HandleMeshSelection(const FMeshData& MeshData);

	UFUNCTION()
	void HandleMaterialSelection(const FMaterialData& MaterialData);

	UFUNCTION()
	void HandleTextureSelection(const FTextureData& TextureData);

	UFUNCTION(BlueprintCallable)
	void Switching();



	
	// ViewPawn Possess Variables And Functions

	UPROPERTY()
	UInputMappingContext* PossessMapping;

	UPROPERTY()
	UInputAction* PossessAction;

	UPROPERTY(EditAnywhere)
	UDataTable* Table;

	UPROPERTY()
	APawn* currentPawn;

	UPROPERTY()
	int32 Index = 0;

	void SpawnAndPossess();

	UPROPERTY(EditAnywhere, category = "PAWN")
	TEnumAsByte<EViewPawnType> ViewPawnType;

	void ActorSpawn(const FMeshData&MeshData);


	// Class Functions 
	AInteractiveArchController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* pawn) override;

	void MapKey(UInputMappingContext* mapping, UInputAction* action, FKey key, bool bNegate = false, bool bSwizzle = false, EInputAxisSwizzle swizzleOrder = EInputAxisSwizzle::YXZ);
};
