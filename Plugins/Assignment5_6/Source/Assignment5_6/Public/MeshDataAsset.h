// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FMeshType {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Property")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Property")
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Property")
	float MinScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Property")
	float MaxSacle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Property")
	float MinRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Property")
	float MaxRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Property")
	float MinDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Property")
	float MaxDistance;
};


UCLASS()
class ASSIGNMENT5_6_API UMeshDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<FMeshType> MeshTypeArray;
};
