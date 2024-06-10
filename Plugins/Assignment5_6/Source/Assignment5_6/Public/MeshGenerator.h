// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshDataAsset.h"
#include "FAsyncScatterTask.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "MeshGenerator.generated.h"

UENUM()
enum class EAreaType : int8
{
	Box ,
	Sphere
};

UCLASS()
class ASSIGNMENT5_6_API AMeshGenerator : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, Category = "Scatter")
	UMeshDataAsset* DataAsset;

	UPROPERTY(EditAnywhere, Category = "Scatter")
	int32 NumberOfInstances = 100;

	UPROPERTY(EditAnywhere ,  BlueprintReadWrite)
	FVector Location;

	UPROPERTY(EditAnywhere ,  BlueprintReadWrite)
	FVector Dimension;

	UFUNCTION(BlueprintCallable)
	void ScatterObjects();

	UPROPERTY(EditAnyWhere , BlueprintReadWrite)
	float ProgressPrecent;

	UPROPERTY()
	TEnumAsByte<EAreaType> AreaType;

	FAsyncTask<FAsyncScatterTask>* AsyncScatterTask;

	UPROPERTY()
	TMap<UStaticMesh*, UHierarchicalInstancedStaticMeshComponent*> HISMComponents;

	UPROPERTY()
	TArray<UHierarchicalInstancedStaticMeshComponent*> ScatteredMeshArray;
	
	void FinishScatter();

	void AddInstances(UStaticMesh* StaticMesh, const FTransform& Transforms, UMaterialInterface* Material);

	AMeshGenerator();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void Tick(float DeltaTime) override;

};
