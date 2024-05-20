// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SplineComponent.h>
#include <Components/SplineMeshComponent.h>
#include "WallSpline.generated.h"

UCLASS()
class MYPLUGIN_API AWallSpline : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	USplineComponent* spline;

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* scene;

	UPROPERTY()
	TArray<USplineMeshComponent*> WallArray;
	
public:
	AWallSpline();

	UPROPERTY()
	TArray<FVector> points;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* staticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* material;

	UFUNCTION(BlueprintCallable)
	void GenerateWalls();
	//void GenerateWalls();
	void generatePoints(FVector& point);


	//void generatePoints();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void DestroyWalls();

	void ClearPoints();

	void AddPoint(FVector point);

	void RemoveLastPoint();

	int32 GetSplinePoints();

};
