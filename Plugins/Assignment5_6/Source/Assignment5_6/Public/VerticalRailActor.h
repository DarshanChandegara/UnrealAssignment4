#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "VerticalRailActor.generated.h"

UENUM()
enum class ETopType : int8 {
	VerticleRail,
	AcornCapital,
	RoundTurnedCapital,
	PyramidTop , 
	RoundedOverTop,
	WindsorTurnedCapital,
	GothicStartTop
};

UCLASS()
class ASSIGNMENT5_6_API AVerticalRailActor : public AActor
{
	GENERATED_BODY()
public:	

	UPROPERTY()
	USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Defaults)
	UProceduralMeshComponent* VerticalRail;	

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	UMaterialInterface* Material_;
	
	AVerticalRailActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;


	// Shape Generations
	UFUNCTION(BlueprintCallable, Category = Defauls)
	void GenerateCube(FVector Dimensions , FVector Offset = FVector(0, 0, 0));

	UFUNCTION(BlueprintCallable, Category = Defauls)
	void GenerateSphere(FVector Dimension, FVector Offset = FVector(0, 0, 0));

	UFUNCTION(BlueprintCallable, Category = Defauls)
	void GeneratePyramid(FVector Dimension, FVector Offset = FVector(0,0,0));

	UFUNCTION(BlueprintCallable, Category = Defauls)
	void GenerateEgg(FVector Dimension, FVector Offset = FVector(0, 0, 0));

	UFUNCTION(BlueprintCallable, Category = Defauls)
	void GenerateCapsule(FVector Dimension, FVector Offset = FVector(0, 0, 0));

	UFUNCTION(BlueprintCallable, Category = Defauls)
	void GenerateBellShape(FVector Location, float BaseRadius, float Height, float RimRadius, float CurvatureFactor, int NumSlices, int NumStacks);

	UFUNCTION(BlueprintCallable, Category = Defauls)
	void GenerateCone(FVector Dimension, FVector Offset = FVector(0,0,0));

	UFUNCTION(BlueprintCallable, Category = Defauls)
	void GenerateDonut(float OuterRadius, float InnerRadius, int NumSegments, int NumSides, const FVector& LocationOffset);

	UFUNCTION(BlueprintCallable)
	void GenerateCircle(int Points, float Radius);

	UFUNCTION(BlueprintCallable)
	void GenerateCylinder(int Points, float Radius , float Height_);
	// Fence Generations

	void Generate(FVector Dimension);

	void GenerateVerticleRail(FVector Dimension);
	void WindsorTurnedCapital(FVector Dimension);
	void RoundTurnedCapital(FVector Dimenstion);
	void AcornCapital(FVector Dimension);
	void RoundedOverTop(FVector Dimension);
	void PyramidTop(FVector Dimension);
	void GothicStarTop(FVector Dimension);

	void SetVerticalMaterial(UMaterialInterface* Material);
	void SetHorizontalMaterial(UMaterialInterface* Material);
	void SetTopMaterial(UMaterialInterface* Material);


	UPROPERTY()
	int32 SphereResolution		;

	UPROPERTY()
    int32 Segments;

    UPROPERTY()
	float SphereRadius;

	UPROPERTY( )
	float Height;

	UPROPERTY( )
	float PyramidHeight;

	UPROPERTY()
	float PyramidBase;	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh")
	TEnumAsByte<ETopType> TopType;

};
