// Fill out your copyright notice in the Description page of Project Settings.


#include "FAsyncScatterTask.h"
#include "CustomPlayerController.h"
#include "MeshGenerator.h"

FAsyncScatterTask::FAsyncScatterTask(AMeshGenerator* InActor, FVector InLocation, FVector InDimesnion)
{
	MeshGenerator = InActor;
	Location = InLocation;
	Dimension = InDimesnion;
}

FVector FAsyncScatterTask::GeneraterandomPointInSphere(FVector Origin, float Radius) {
	float theta = 2.0f * PI * static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX); // Angle in XY plane
	float phi = std::acos(2.0f * static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) - 1.0f); // Angle from Z axis
	float u = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX); // Uniform random variable

	// Convert to spherical coordinates
	float r = Radius * std::cbrt(u); // Scale radius by cube root of a uniform random variable

	// Convert spherical coordinates to Cartesian coordinates
	float x = r * std::sin(phi) * std::cos(theta);
	float y = r * std::sin(phi) * std::sin(theta);
	float z = r * std::cos(phi);

	return Origin + FVector(x, y, z);
}

void FAsyncScatterTask::DoWork()
{
	if (MeshGenerator.IsValid())
	{
		if (UMeshDataAsset* DataAsset = MeshGenerator->DataAsset)
		{
			TArray<FMeshType> MeshArray = DataAsset->MeshTypeArray;

			for (int iIndex = 0; MeshGenerator.IsValid() && (iIndex < MeshGenerator->NumberOfInstances); iIndex++)
			{
				float RandomIndex = FMath::RandRange(0, DataAsset->MeshTypeArray.Num() - 1);

				FMeshType CurrentMeshType = MeshArray[RandomIndex];

				UStaticMesh* CurrentMesh = CurrentMeshType.StaticMesh;
				UMaterialInterface* CurrentMaterial = CurrentMeshType.Material;
				FTransform Transform;

				FVector BoundingExtent = Dimension;
				FVector Origin = Location;
				FBox BoundingBox(Origin - BoundingExtent, Origin + BoundingExtent);

				float RandomValue = FMath::RandRange(CurrentMeshType.MinScale, CurrentMeshType.MaxSacle);
				float RandomRotation = FMath::RandRange(CurrentMeshType.MinRotation, CurrentMeshType.MaxRotation);

				if(MeshGenerator.IsValid())
				{
					if (MeshGenerator->AreaType == EAreaType::Box)
					{
						FVector Position = FMath::RandPointInBox(BoundingBox);
						Transform = (FTransform(FRotator(RandomRotation), Position, FVector(RandomValue)));
					}
					else {
						FVector Position = GeneraterandomPointInSphere(Origin, MeshGenerator->Dimension.X);
						Transform = (FTransform(FRotator(RandomRotation), Position, FVector(RandomValue)));
					}
				}
				if (MeshGenerator.IsValid()) {
					MeshGenerator->ProgressPrecent = static_cast<float>(iIndex) / static_cast<float>(MeshGenerator->NumberOfInstances - 1);
					MeshGenerator->AddInstances(CurrentMesh, Transform, CurrentMaterial);
				}
				FPlatformProcess::Sleep(0.005f);
			}
		}
	}


}

