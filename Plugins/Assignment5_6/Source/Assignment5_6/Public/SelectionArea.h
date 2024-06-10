// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "SelectionArea.generated.h"

UCLASS()
class ASSIGNMENT5_6_API ASelectionArea : public AActor
{
	GENERATED_BODY()

	
public:	
	UPROPERTY(EditAnywhere )
	UProceduralMeshComponent* AreaMesh;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	UMaterialInterface* Material;

	ASelectionArea();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void GenerateSphere(float Radius);
	
	UFUNCTION(BlueprintCallable)
	void GenerateBox(FVector Dimension);

};
