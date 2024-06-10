// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionArea.h"

ASelectionArea::ASelectionArea()
{
	PrimaryActorTick.bCanEverTick = true;

	AreaMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	AreaMesh->SetupAttachment(GetRootComponent());

}

void ASelectionArea::BeginPlay()
{
	Super::BeginPlay();
}

void ASelectionArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASelectionArea::GenerateSphere(float Radius)
{
	if (AreaMesh)
	{
		AreaMesh->ClearAllMeshSections();

		TArray<FVector> Vertices;
		TArray<int32> Triangles;
		TArray<FVector> Normals;
		TArray<FVector2D> UV0;
		TArray<FProcMeshTangent> Tangents;

		float SphereResolution = 50.f;

		for (int i = 0; i < SphereResolution; i++) {

			float Theta = i * PI / (SphereResolution - 1);
			float SinTheta = FMath::Sin(Theta);
			float CosTheta = FMath::Cos(Theta);

			for (int j = 0; j < SphereResolution; j++) {
				float Phi = 2 * j * PI / (SphereResolution - 1);
				float SinPhi = FMath::Sin(Phi);
				float CosPhi = FMath::Cos(Phi);

				FVector vertex = FVector(SinTheta * CosPhi, SinTheta * SinPhi, CosTheta) * Radius;
				Vertices.Add(vertex);

				Normals.Add(vertex.GetSafeNormal());
				UV0.Add(FVector2D(-Phi / (2 * PI), Theta / PI));

				if (i < SphereResolution - 1 && j < SphereResolution - 1) {
					int current = i * SphereResolution + j;
					int next = current + SphereResolution;

					Triangles.Add(current);
					Triangles.Add(current + 1);
					Triangles.Add(next);

					Triangles.Add(current + 1);
					Triangles.Add(next + 1);
					Triangles.Add(next);
				}
			}
		}

		AreaMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, TArray<FLinearColor>(), Tangents, true);
		AreaMesh->SetMaterial(0, Material);
	}
}

void ASelectionArea::GenerateBox(FVector Dimension)
{
	if (AreaMesh)
	{
		AreaMesh->ClearAllMeshSections();
		TArray<FVector> Vertices;
		float L = (Dimension.X / 2.0f);
		float W = (Dimension.Y / 2.0f);
		float H = (Dimension.Z / 2.0f);

		// Front Face
		Vertices.Add(FVector(-L, W, H)); // 0
		Vertices.Add(FVector(L, W, H));  // 1
		Vertices.Add(FVector(L, -W, H));   // 2
		Vertices.Add(FVector(-L, -W, H));  // 3

		// Back Face
		Vertices.Add(FVector(L, W, -H)); // 4
		Vertices.Add(FVector(-L, W, -H));  // 5
		Vertices.Add(FVector(-L, -W, -H));   // 6
		Vertices.Add(FVector(L, -W, -H));  // 7

		// Top Face
		Vertices.Add(FVector(-L, W, -H)); // 8
		Vertices.Add(FVector(L, W, -H));  // 9
		Vertices.Add(FVector(L, W, H));   // 10
		Vertices.Add(FVector(-L, W, H));  // 11

		// Bottom Face
		Vertices.Add(FVector(-L, -W, H)); // 12
		Vertices.Add(FVector(L, -W, H));  // 13
		Vertices.Add(FVector(L, -W, -H));   // 14
		Vertices.Add(FVector(-L, -W, -H));  // 15

		// Right Face
		Vertices.Add(FVector(L, W, H)); // 16
		Vertices.Add(FVector(L, W, -H));  // 17
		Vertices.Add(FVector(L, -W, -H));   // 18
		Vertices.Add(FVector(L, -W, H));  // 19

		// Left Face
		Vertices.Add(FVector(-L, W, -H)); // 20
		Vertices.Add(FVector(-L, W, H));  // 21
		Vertices.Add(FVector(-L, -W, H));   // 22
		Vertices.Add(FVector(-L, -W, -H));  // 23


		TArray<int32> Triangles = {
		0, 1, 2, 0, 2, 3, // Front face
		4, 5, 6, 4, 6, 7, // Back face
		8, 9, 10, 8, 10, 11, // Top face
		12, 13, 14, 12, 14, 15, // Bottom face
		16, 17, 18, 16, 18, 19, // Right face
		20, 21, 22, 20, 22, 23 // Left face
		};


		TArray<FVector> Normals = {
			FVector(0,0,1) ,FVector(0,0,1),FVector(0,0,1),FVector(0,0,1), // Front
			FVector(0,0,-1) ,FVector(0,0,-1) ,FVector(0,0,-1) ,FVector(0,0,-1) , // Back
			FVector(0,1,0) ,FVector(0,1,0) , FVector(0,1,0) ,FVector(0,1,0) , //Top
			FVector(0,-1,0) , FVector(0,-1,0) ,FVector(0,-1,0) ,FVector(0,-1,0) ,// Bottom
			FVector(1,0,0) ,FVector(1,0,0) , FVector(1,0,0) ,FVector(1,0,0) ,// Right
			FVector(-1,0,0) ,FVector(-1,0,0) ,FVector(-1,0,0) ,FVector(-1,0,0)   // Left

		};


		TArray<FVector2D> UVs{
			FVector2D(0,0) , FVector2D(-1,0) , FVector2D(-1,1) , FVector2D(0,1) ,
			FVector2D(1,0) , FVector2D(0,0) , FVector2D(0,1) , FVector2D(1,1) ,
			FVector2D(0,1) , FVector2D(1,1) , FVector2D(1,0) , FVector2D(0,0) ,
			FVector2D(1,0) , FVector2D(0,0) , FVector2D(0,1) , FVector2D(1,1) ,
			FVector2D(0,0) , FVector2D(0,1) , FVector2D(1,1) , FVector2D(1,0) ,
			FVector2D(0,1) , FVector2D(0,0) , FVector2D(-1,0) , FVector2D(-1,1) ,

		};

		AreaMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
		AreaMesh->SetMaterial(0, Material);
	}

}

