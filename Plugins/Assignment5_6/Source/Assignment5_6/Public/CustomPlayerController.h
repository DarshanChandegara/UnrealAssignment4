// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SelectionArea.h"
#include "MeshGenerator.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "CustomPlayerController.generated.h"

UCLASS()
class ASSIGNMENT5_6_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadWrite)
	bool isMoving = true;

	UPROPERTY(BlueprintReadWrite)
	ASelectionArea* AreaActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASelectionArea> SelectionAreaClass;

	UPROPERTY(BlueprintReadWrite)
	AMeshGenerator* MeshGeneratorActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMeshGenerator> MeshGeneratorClass;

	UPROPERTY()
	UInputMappingContext* Mapping;

	UPROPERTY()
	UInputAction* LeftClickAction;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	float Radius;

	UPROPERTY( EditAnywhere ,BlueprintReadWrite)
	float Lentgh;

	UPROPERTY( EditAnywhere ,BlueprintReadWrite)
	float Width;

	UPROPERTY( EditAnywhere ,BlueprintReadWrite)
	float Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfInstances;

	ACustomPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable)
	void GenerateAreaShape(int index);

	UFUNCTION(BlueprintCallable)
	void HandleButtonClick(int index);

	void LeftClick();

};
