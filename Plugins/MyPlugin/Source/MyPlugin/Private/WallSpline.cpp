// Fill out your copyright notice in the Description page of Project Settings.


#include "WallSpline.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AWallSpline::AWallSpline()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Magenta, "Constructor Wall");

	scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = scene;
	spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	spline->SetupAttachment(scene);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Architecture/Wall_400x400.Wall_400x400'"));

	if (MeshAsset.Succeeded()) {
		staticMesh = MeshAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Script/Engine.Material'/Engine/MaterialTemplates/PerfTricks/M_SpecularUsingDiffuse.M_SpecularUsingDiffuse'"));

	if (MeshAsset.Succeeded()) {
		material = MaterialAsset.Object;
	}

}

void AWallSpline::GenerateWalls()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Magenta, "Genereate Wall"); 
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Magenta, FString::FromInt(points.Num()));

	DestroyWalls();
	spline->SetSplinePoints(points, ESplineCoordinateSpace::World);

	int Nopoints = spline->GetNumberOfSplinePoints();

	for (int i = 0; i < Nopoints - 1; i++) {
		FVector startLocation, startTangent;
		spline->GetLocationAndTangentAtSplinePoint(i, startLocation, startTangent, ESplineCoordinateSpace::World);

		FVector endLocation, endTangent;
		spline->GetLocationAndTangentAtSplinePoint(i + 1, endLocation, endTangent, ESplineCoordinateSpace::World);

		USplineMeshComponent* mesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		mesh->RegisterComponent();
		mesh->SetMobility(EComponentMobility::Movable);
		mesh->SetStaticMesh(staticMesh);
		mesh->SetMaterial(0, material);
		mesh->SetStartAndEnd(startLocation, startTangent, endLocation, endTangent);
		mesh->AttachToComponent(spline, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		WallArray.Add(mesh);
	}
}

void AWallSpline::generatePoints(FVector& point)
{
	spline->AddSplinePoint(point, ESplineCoordinateSpace::World);
	//GenerateWalls();
}

void AWallSpline::BeginPlay()
{
	Super::BeginPlay();
	spline->ClearSplinePoints();
}

void AWallSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWallSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AWallSpline::DestroyWalls()
{
	for (int i = 0; i < WallArray.Num(); i++) {
		if (WallArray[i]) {
			WallArray[i]->DestroyComponent();
		}
	}
	WallArray.Empty();
}

void AWallSpline::ClearPoints()
{
	points.Empty();
	spline->ClearSplinePoints();
}

void AWallSpline::AddPoint(FVector point)
{
	points.Add(point);
}

void AWallSpline::RemoveLastPoint()
{
	if (points.Num() > 0)
	{
		points.RemoveAt(points.Num() - 1);
	}
}

int32 AWallSpline::GetSplinePoints()
{
	return points.Num();
}
