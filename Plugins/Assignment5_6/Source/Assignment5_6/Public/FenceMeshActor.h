#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SplineComponent.h>
#include "VerticalRailActor.h"
#include "FenceMeshActor.generated.h"

USTRUCT(BlueprintType)
struct FFenceProperties {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "FenceProperty")
	float Height;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "FenceProperty")
	float Width;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "FenceProperty")
	float Length;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "FenceProperty")
	float Spacing;
};

UCLASS()
class ASSIGNMENT5_6_API AFenceMeshActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FVector> points;

	UPROPERTY()
	TArray<UStaticMeshComponent*> StaticMeshRailArray;


public:	
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TileX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TileY;

	UPROPERTY()
	USceneComponent* Scene;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	USplineComponent* Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AVerticalRailActor> VerticalRailActorClass;

	UPROPERTY()
	TArray<AVerticalRailActor*> RailArray;
	
	AFenceMeshActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Mesh")
	FFenceProperties FenceProperties;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void GenerateRails();
	void DestroyRails();
	void GenerateSplinePoints();
	void AddStaticRails();
	void ClearStaticRails();
	void ReplaceStaticMeshWithProceduralMesh();
};
