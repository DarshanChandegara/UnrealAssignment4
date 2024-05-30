// Fill out your copyright notice in the Description page of Project Settings.


#include "VerticalRailActor.h"
#include <Kismet/KismetMathLibrary.h>

AVerticalRailActor::AVerticalRailActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	VerticalRail = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	VerticalRail->SetupAttachment(Scene);

	SphereResolution = 32;
	SphereRadius = 25.0f;

	PyramidHeight = 25.0f;
	PyramidBase = 25.0f;
	//TileX = 1.5f;
	//TileY = 1.5f;

}

void AVerticalRailActor::BeginPlay()
{
	Super::BeginPlay();

}

void AVerticalRailActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVerticalRailActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//UE_LOG(LogTemp, Warning, TEXT("The value of Segments is: %d"), Segments);
	Segments = 0;
	FVector Dimension(25, 25, 250);
	switch(TopType) {
	case ETopType::RoundTurnedCapital:
		VerticalRail->ClearAllMeshSections();
		RoundTurnedCapital(Dimension);
		break;

	case ETopType::PyramidTop:
		VerticalRail->ClearAllMeshSections();
		PyramidTop(Dimension);
		break;

	case ETopType::AcornCapital:
		VerticalRail->ClearAllMeshSections();
		AcornCapital(Dimension);
		break;

	case ETopType::RoundedOverTop:
		VerticalRail->ClearAllMeshSections();
		RoundedOverTop(Dimension);
		break;

	case ETopType::WindsorTurnedCapital:
		VerticalRail->ClearAllMeshSections();
		WindsorTurnedCapital(Dimension);
		break;

	case ETopType::GothicStartTop:
		VerticalRail->ClearAllMeshSections();
		GothicStarTop(Dimension);
		break;

	case ETopType::VerticleRail:
		VerticalRail->ClearAllMeshSections();
		GenerateVerticleRail(Dimension);
		break;
	}

}




void AVerticalRailActor::GenerateCube(FVector Dimensions, FVector Offset)
{
	//ProceduralMesh->ClearAllMeshSections();

	////// Define the vertices for a cube
	TArray<FVector> Vertices;
	float L = (Dimensions.X / 2.0f);
	float W = (Dimensions.Y / 2.0f);
	float H = (Dimensions.Z / 2.0f);

	// Front Face
	Vertices.Add(FVector(-L + Offset.X, W + Offset.Y, H + Offset.Z)); // 0
	Vertices.Add(FVector(L + Offset.X, W + Offset.Y, H + Offset.Z));  // 1
	Vertices.Add(FVector(L + Offset.X, -W + Offset.Y, H + Offset.Z));   // 2
	Vertices.Add(FVector(-L + Offset.X, -W + Offset.Y, H + Offset.Z));  // 3

	// Back Face
	Vertices.Add(FVector(L + Offset.X, W + Offset.Y, -H + Offset.Z)); // 4
	Vertices.Add(FVector(-L + Offset.X, W + Offset.Y, -H + Offset.Z));  // 5
	Vertices.Add(FVector(-L + Offset.X, -W + Offset.Y, -H + Offset.Z));   // 6
	Vertices.Add(FVector(L + Offset.X, -W + Offset.Y, -H + Offset.Z));  // 7

	// Top Face
	Vertices.Add(FVector(-L + Offset.X, W + Offset.Y, -H + Offset.Z)); // 8
	Vertices.Add(FVector(L + Offset.X, W + Offset.Y, -H + Offset.Z));  // 9
	Vertices.Add(FVector(L + Offset.X, W + Offset.Y, H + Offset.Z));   // 10
	Vertices.Add(FVector(-L + Offset.X, W + Offset.Y, H + Offset.Z));  // 11

	// Bottom Face
	Vertices.Add(FVector(-L + Offset.X, -W + Offset.Y, H + Offset.Z)); // 12
	Vertices.Add(FVector(L + Offset.X, -W + Offset.Y, H + Offset.Z));  // 13
	Vertices.Add(FVector(L + Offset.X, -W + Offset.Y, -H + Offset.Z));   // 14
	Vertices.Add(FVector(-L + Offset.X, -W + Offset.Y, -H + Offset.Z));  // 15

	// Right Face
	Vertices.Add(FVector(L + Offset.X, W + Offset.Y, H + Offset.Z)); // 16
	Vertices.Add(FVector(L + Offset.X, W + Offset.Y, -H + Offset.Z));  // 17
	Vertices.Add(FVector(L + Offset.X, -W + Offset.Y, -H + Offset.Z));   // 18
	Vertices.Add(FVector(L + Offset.X, -W + Offset.Y, H + Offset.Z));  // 19

	// Left Face
	Vertices.Add(FVector(-L + Offset.X, W + Offset.Y, -H + Offset.Z)); // 20
	Vertices.Add(FVector(-L + Offset.X, W + Offset.Y, H + Offset.Z));  // 21
	Vertices.Add(FVector(-L + Offset.X, -W + Offset.Y, H + Offset.Z));   // 22
	Vertices.Add(FVector(-L + Offset.X, -W + Offset.Y, -H + Offset.Z));  // 23

	//Vertices.Add(FVector(-L, -W, H)); // 0
	//Vertices.Add(FVector(L, -W, H));  // 1
	//Vertices.Add(FVector(L, -W, -H));   // 2
	//Vertices.Add(FVector(-L, -W, -H));  // 3

	//// Back Face
	//Vertices.Add(FVector(L, -W, -H)); // 4
	//Vertices.Add(FVector(-L, -W, -H));  // 5
	//Vertices.Add(FVector(-L, W, -H));   // 6
	//Vertices.Add(FVector(L, W, -H));  // 7

	//// Top Face
	//Vertices.Add(FVector(-L, -W, -H)); // 8
	//Vertices.Add(FVector(L, -W, -H));  // 9
	//Vertices.Add(FVector(L, -W, H));   // 10
	//Vertices.Add(FVector(-L, -W, H));  // 11

	//// Bottom Face
	//Vertices.Add(FVector(-L, W, H)); // 12
	//Vertices.Add(FVector(L, W, H));  // 13
	//Vertices.Add(FVector(L, W, -H));   // 14
	//Vertices.Add(FVector(-L, W, -H));  // 15

	//// Right Face
	//Vertices.Add(FVector(L, -W, H)); // 16
	//Vertices.Add(FVector(L, -W, -H));  // 17
	//Vertices.Add(FVector(L, W, -H));   // 18
	//Vertices.Add(FVector(L, W, H));  // 19

	//// Left Face
	//Vertices.Add(FVector(-L, -W, -H)); // 20
	//Vertices.Add(FVector(-L, -W, H));  // 21
	//Vertices.Add(FVector(-L, W, H));   // 22
	//Vertices.Add(FVector(-L, W, -H));  // 23


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

	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, FString::FromInt(Segments));
	UE_LOG(LogTemp, Warning, TEXT("The value of Segments is: %d"), Segments);

	VerticalRail->CreateMeshSection_LinearColor(Segments++, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	//if (Segments == 0)
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT("Inside "));

	//	//UMaterialInstanceDynamic* VerticalMaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
	//	////TileX = Dimensions.X / 25;
	//	////TileY = Dimensions.Z / 50;
	//	//TileX = VerticalRail->GetRelativeScale3D().Y ;
	//	//TileY = VerticalRail->GetComponentTransform().GetScale3D().Z*5;
	//	//
	//	//if (VerticalMaterialInstance) {
	//	//	UE_LOG(LogTemp, Warning, TEXT("Inside Material 1"));
	//	//}
	//	//VerticalMaterialInstance->SetScalarParameterValue(TEXT("TileX"), TileX);
	//	//VerticalMaterialInstance->SetScalarParameterValue(TEXT("TileY"), TileY);
	//	//VerticalRail->SetMaterial(Segments++, VerticalMaterialInstance);
	//}
	//else if(Segments==1 || Segments==2)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Outside ")); 
	//	TileX = Dimensions.X / 50;
	//	TileY = Dimensions.Z / 25;

	//	UMaterialInstanceDynamic* HorizontalMaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
	//	if (HorizontalMaterialInstance) {
	//		UE_LOG(LogTemp, Warning, TEXT("Inside Material 2"));
	//	}
	//	HorizontalMaterialInstance->SetScalarParameterValue(TEXT("TileX"), TileX);
	//	HorizontalMaterialInstance->SetScalarParameterValue(TEXT("TileY"), TileY);
	//	VerticalRail->SetMaterial(Segments++, HorizontalMaterialInstance);
	//}
	//else {
	//	TileX = 1;
	//	TileY = 1;

	//	UMaterialInstanceDynamic* HorizontalMaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
	//	if (HorizontalMaterialInstance) {
	//		UE_LOG(LogTemp, Warning, TEXT("Inside Material 2"));
	//	}
	//	HorizontalMaterialInstance->SetScalarParameterValue(TEXT("TileX"), TileX);
	//	HorizontalMaterialInstance->SetScalarParameterValue(TEXT("TileY"), TileY);
	//	VerticalRail->SetMaterial(Segments++, HorizontalMaterialInstance);
	//	//VerticalRail->SetMaterial(Segments++, Material2);
	//}

}

void AVerticalRailActor::GenerateSphere(FVector Dimension, FVector Offset)
{

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> Tangents;

	SphereRadius = (Dimension.X * 3) / 5;

	for (int i = 0; i < SphereResolution; i++) {

		float Theta = i * PI / (SphereResolution - 1);
		float SinTheta = FMath::Sin(Theta);
		float CosTheta = FMath::Cos(Theta);

		for (int j = 0; j < SphereResolution; j++) {
			float Phi = 2 * j * PI / (SphereResolution - 1);
			float SinPhi = FMath::Sin(Phi);
			float CosPhi = FMath::Cos(Phi);

			FVector vertex = FVector(SinTheta * CosPhi, SinTheta * SinPhi, CosTheta) * SphereRadius;
			Vertices.Add(vertex + Offset);

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
	
	//GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, FString::FromInt(Segments));
	UE_LOG(LogTemp, Warning, TEXT("The value of Segments is: %d"), Segments);

	VerticalRail->CreateMeshSection_LinearColor(Segments++, Vertices, Triangles, Normals, UV0, TArray<FLinearColor>(), Tangents, true);

}

void AVerticalRailActor::GeneratePyramid(FVector Dimension, FVector Offset)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV;
	TArray<FProcMeshTangent> Tangents;

	PyramidBase = Dimension.X;
	PyramidHeight = Dimension.X*1.2;


	Vertices.Add(FVector(0, 0, PyramidHeight) + Offset); // Top vertex
	Vertices.Add(FVector((-PyramidBase / 2) , (-PyramidBase / 2) ,  0 ) + Offset); // Base vertex 1
	Vertices.Add(FVector((PyramidBase / 2) , (-PyramidBase / 2) ,  0 ) + Offset); // Base vertex 2
	Vertices.Add(FVector((PyramidBase / 2) , (PyramidBase / 2) , 0) + Offset); // Base vertex 3
	Vertices.Add(FVector((-PyramidBase / 2) , (PyramidBase / 2) ,  0 ) + Offset); // Base vertex 4

	// Define triangles for each face
	Triangles.Append({ 0, 1, 2 }); // Front face
	Triangles.Append({ 0, 2, 1 }); // Front face

	Triangles.Append({ 0, 2, 3 }); // Right face
	Triangles.Append({ 0, 3, 2 }); // Right face

	Triangles.Append({ 0, 3, 4 }); // Back face
	Triangles.Append({ 0, 4, 3 }); // Back face

	Triangles.Append({ 0, 4, 1 }); // Left face
	Triangles.Append({ 0, 1, 4 }); // Left face

	// Define triangles for the base
	Triangles.Append({ 1, 4, 3 });
	Triangles.Append({ 1, 3, 4 });

	Triangles.Append({ 1, 3, 2 });
	Triangles.Append({ 1, 2, 3 });

	// Calculate normals for each vertex
	for (int32 i = 0; i < Vertices.Num(); ++i)
	{
		Normals.Add(FVector(0, 0, 1)); // Just using UpVector as placeholder, calculate actual normals based on your needs
	}

	// Add UVs for texturing (simplified for this example)
	UV.Add(FVector2D(0.5f, 0.5f)); // Top vertex
	UV.Add(FVector2D(0.0f, 0.0f)); // Base vertex 1
	UV.Add(FVector2D(1.0f, 0.0f)); // Base vertex 2
	UV.Add(FVector2D(1.0f, 1.0f)); // Base vertex 3
	UV.Add(FVector2D(0.0f, 1.0f)); // Base vertex 4

	// Tangents (simplified for this example)
	for (int32 i = 0; i < Vertices.Num(); ++i)
	{
		Tangents.Add(FProcMeshTangent(0.0f, 1.0f, 0.0f));
	}


	UE_LOG(LogTemp, Warning, TEXT("The value of Segments is: %d"), Segments);

	VerticalRail->CreateMeshSection_LinearColor(Segments++, Vertices, Triangles, Normals, UV, TArray<FLinearColor>(), Tangents, true);

}

void AVerticalRailActor::GenerateEgg(FVector Dimension, FVector Offset)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	int32 LatitudeSegments = 25;
	int32 LongitudeSegments = 25;
	float Radius = Dimension.X * 3 / 5;
	float Height1 = Dimension.X;

	for (int32 Lat = 0; Lat <= LatitudeSegments; Lat++)
	{
		float Theta = Lat * PI / LatitudeSegments;
		float SinTheta = FMath::Sin(Theta);
		float CosTheta = FMath::Cos(Theta);

		for (int32 Lon = 0; Lon <= LongitudeSegments; Lon++)
		{
			float Phi = Lon * 2 * PI / LongitudeSegments;
			float SinPhi = FMath::Sin(Phi);
			float CosPhi = FMath::Cos(Phi);

			float X = (Radius * SinTheta * CosPhi);
			float Y = (Radius * SinTheta * SinPhi);
			float Z = (Height1 * CosTheta);

			Vertices.Add(FVector(X, Y, Z) + Offset);
			UV0.Add(FVector2D(-(float)Lon / (float)LongitudeSegments, (float)Lat / (float)LatitudeSegments));

			FColor Color = (Lat == LatitudeSegments / 2) ? FColor::Red : FColor::White;
			VertexColors.Add(Color);
		}
	}

	for (int32 Lat = 0; Lat < LatitudeSegments; Lat++)
	{
		for (int32 Lon = 0; Lon < LongitudeSegments; Lon++)
		{
			int32 First = (Lat * (LongitudeSegments + 1)) + Lon;
			int32 Second = First + LongitudeSegments + 1;

			Triangles.Add(First);
			Triangles.Add(First + 1);
			Triangles.Add(Second);


			Triangles.Add(Second);
			Triangles.Add(First + 1);
			Triangles.Add(Second + 1);
		}
	}

	for (int i = 0; i < Vertices.Num(); i++) {
		Normals.Add(FVector::ZeroVector);
	}

	for (int i = 0; i < Triangles.Num(); i += 3) {

		if (i + 2 < Triangles.Num()) {

			FVector vec1 = Vertices[Triangles[i + 1]] - Vertices[Triangles[i]];
			FVector vec2 = Vertices[Triangles[i + 2]] - Vertices[Triangles[i]];

			auto Normal = FVector::CrossProduct(vec2, vec1).GetSafeNormal();

			Normals[Triangles[i]] += Normal;
			Normals[Triangles[i + 1]] += Normal;
			Normals[Triangles[i + 2]] += Normal;

		}
	}


	for (int i = 0; i < Normals.Num(); i++) {
		Normals[i].Normalize();
	}

	VerticalRail->CreateMeshSection(Segments++, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
	VerticalRail->ContainsPhysicsTriMeshData(true);
}

void AVerticalRailActor::GenerateCapsule(FVector Dimension, FVector Offset)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	const int32 NumSegments = 20; // Number of segments in the semicircle
	const float DeltaTheta = 180.0f / (NumSegments - 1);

	Vertices.Add(FVector{ 0,0,0 } + Offset);
	Vertices.Add(FVector{ 0,  Dimension.Y / 2,0 } + Offset);
	Vertices.Add(FVector{ 0,-Dimension.Y / 2,0 } + Offset);

	UVs.Add(FVector2D(0.5f, 0.5f)); // Center
	UVs.Add(FVector2D(0.5f, 0.5f)); // Top middle (on front)
	UVs.Add(FVector2D(0.5f, 0.0f)); // Top middle (on back)


	// Top vertices of the semicircles
	for (int32 i = 0; i < NumSegments; i++)
	{
		float AngleRad = FMath::DegreesToRadians(i * DeltaTheta);
		float X = SphereRadius * FMath::Cos(AngleRad);
		float Z = SphereRadius * FMath::Sin(AngleRad);

		// Vertices on Front
		Vertices.Add(FVector(X, Dimension.Y / 2, Z) + Offset);

		// Vertices on Back
		Vertices.Add(FVector(X, -Dimension.Y / 2, Z) + Offset);


		// Compute UVs for each vertex
		float U = 0.5f + (X / (2 * SphereRadius)); // Normalize X to [0, 1]
		float V_Front = 0.5f + (Z / (2 * SphereRadius)); // Normalize Z to [0, 1] for front
		float V_Back = 0.5f - (Z / (2 * SphereRadius)); // Normalize Z to [0, 1] for back

		UVs.Add(FVector2D(U, V_Front)); // UV for front vertices
		UVs.Add(FVector2D(U, V_Back));  // UV for back vertices


	}

	// Add triangles for semicircles (front faces)
	{
		int32 CurrentVertexIndex = 3;

		for (int32 i = 0; i < NumSegments - 1; i++)
		{
			Triangles.Add(1);
			Triangles.Add(CurrentVertexIndex);
			Triangles.Add(CurrentVertexIndex + 2);
			CurrentVertexIndex += 2;
		}
	}


	// Add triangles for semicircles (Back faces)

	{
		int32 CurrentVertexIndex = 4;

		for (int32 i = 0; i < NumSegments - 1; i++)
		{
			Triangles.Add(2);
			Triangles.Add(CurrentVertexIndex + 2);
			Triangles.Add(CurrentVertexIndex);
			CurrentVertexIndex += 2;
		}
	}


	Normals.Add({ 0,0,-1 });
	Normals.Add({ 0,1,0 });
	Normals.Add({ 0,-1,0 });


	// Calculate normals and tangents
	for (int32 i = 3; i < Vertices.Num(); i++)
	{
		if (i % 2 != 0)
		{
			Normals.Add({ 0,1,0 });
		}
		else
		{
			Normals.Add({ 0, -1 ,0 });
		}


	}


	{
		int32 CurrentVertexIndex = 3;

		for (int32 i = 0; i < (NumSegments - 1); i++)
		{
			Triangles.Add(CurrentVertexIndex);
			Triangles.Add(CurrentVertexIndex + 1);
			Triangles.Add(CurrentVertexIndex + 2);

			Triangles.Add(CurrentVertexIndex + 2);
			Triangles.Add(CurrentVertexIndex + 1);
			Triangles.Add(CurrentVertexIndex + 3);


			CurrentVertexIndex += 2;
		}
	}


	auto OldTotalVertices = Vertices.Num();

	for (int32 i = 3; i < OldTotalVertices; ++i)
	{
		auto Vertex = Vertices[i];
		Vertices.Add(Vertex);

		auto UV = UVs[i];

		UVs.Add(UV);

	}


	{
		int32 CurrentVertexIndex = OldTotalVertices;

		for (int32 i = 0; i < OldTotalVertices - 3; i++)
		{
			Triangles.Add(CurrentVertexIndex);
			Triangles.Add(CurrentVertexIndex + 1);
			Triangles.Add(CurrentVertexIndex + 2);

			Triangles.Add(CurrentVertexIndex + 2);
			Triangles.Add(CurrentVertexIndex + 1);
			Triangles.Add(CurrentVertexIndex + 3);


			CurrentVertexIndex += 2;
		}
	}


	{
		int32 CurrentVertexIndex = OldTotalVertices;
		// Calculate normals and tangents
		for (int32 i = 0; i < OldTotalVertices - 3; i++)
		{
			Normals.Add(UKismetMathLibrary::GetDirectionUnitVector(Vertices[0], Vertices[CurrentVertexIndex]));
			CurrentVertexIndex++;
		}
	}
	//for (int i = 0; i < Vertices.Num(); i++) {
	//	Normals.Add(FVector::ZeroVector);
	//}

	//for (int i = 0; i < Triangles.Num(); i += 3) {

	//	if (i + 2 < Triangles.Num()) {

	//		FVector vec1 = Vertices[Triangles[i + 1]] - Vertices[Triangles[i]];
	//		FVector vec2 = Vertices[Triangles[i + 2]] - Vertices[Triangles[i]];

	//		auto Normal = FVector::CrossProduct(vec2, vec1).GetSafeNormal();

	//		Normals[Triangles[i]] += Normal;
	//		Normals[Triangles[i + 1]] += Normal;
	//		Normals[Triangles[i + 2]] += Normal;

	//	}
	//}


	//for (int i = 0; i < Normals.Num(); i++) {
	//	Normals[i].Normalize();
	//}
	VerticalRail->CreateMeshSection(Segments++, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
	VerticalRail->ContainsPhysicsTriMeshData(true);
}

void AVerticalRailActor::GenerateBellShape(FVector Location, float BaseRadius, float Height1, float RimRadius, float CurvatureFactor, int NumSlices, int NumStacks)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> Colors;

	float Theta, Phi;
	float DeltaTheta = (2.0f * PI) / NumSlices;
	float DeltaPhi = PI / (NumStacks - 1);

	// Generate vertices
	for (int i = 0; i < NumStacks; i++)
	{
		Phi = i * DeltaPhi;

		// Bell profile function for radius interpolation with curvature
		float t = Phi / (PI / 2.0f);
		float CurrentRadius = FMath::Lerp(BaseRadius, RimRadius, FMath::Sin(t * CurvatureFactor)) + 2.5f;

		for (int j = 0; j < NumSlices; j++)
		{
			Theta = j * DeltaTheta;

			float X = CurrentRadius * FMath::Sin(Theta);
			float Y = CurrentRadius * FMath::Cos(Theta);
			float Z = Height1 * (t - 1.0f); // Z decreases with height for bell shape


			// Apply location offset
			X += Location.X;
			Y += Location.Y;
			Z += Location.Z;

			// Invert Z-coordinate


			Vertices.Add(FVector(X, Y, Z + (Height1)));

			// Calculate normals for lighting (approximate)
			Normals.Add(FVector(X, Y, Z).GetSafeNormal());

			// UV mapping
			UVs.Add(FVector2D((float)j / (NumSlices - 1), -(float)i / (NumStacks - 1)));

			// Tangents (optional)
			Tangents.Add(FProcMeshTangent(-FMath::Sin(Theta), FMath::Cos(Theta), 0.0f));
		}
	}

	// Generate triangles (quads converted to triangles)
	for (int i = 0; i < NumStacks - 1; i++)
	{
		for (int j = 0; j < NumSlices - 1; j++)
		{
			int BottomLeft = i * NumSlices + j;
			int BottomRight = BottomLeft + 1;
			int TopLeft = BottomLeft + NumSlices;
			int TopRight = TopLeft + 1;

			// First triangle (bottom left to top left to top right)
			Triangles.Add(BottomLeft);
			Triangles.Add(TopLeft);
			Triangles.Add(BottomRight);

			Triangles.Add(BottomLeft);
			Triangles.Add(BottomRight);
			Triangles.Add(TopLeft);

			// Second triangle (top left to top right to bottom right)
			Triangles.Add(TopLeft);
			Triangles.Add(TopRight);
			Triangles.Add(BottomRight);

			Triangles.Add(TopLeft);
			Triangles.Add(BottomRight);
			Triangles.Add(TopRight);
		}

		// Handle wrap-around triangle for each stack
		int BottomLeftWrap = (i * NumSlices) + (NumSlices - 1);
		int BottomRightWrap = i * NumSlices;
		int TopLeftWrap = BottomLeftWrap + NumSlices;
		int TopRightWrap = BottomRightWrap + NumSlices;

		// First triangle (bottom left to top left to top right)
		Triangles.Add(BottomLeftWrap);
		Triangles.Add(TopLeftWrap);
		Triangles.Add(BottomRightWrap);

		Triangles.Add(BottomLeftWrap);
		Triangles.Add(BottomRightWrap);
		Triangles.Add(TopLeftWrap);

		// Second triangle (top left to top right to bottom right)
		Triangles.Add(TopLeftWrap);
		Triangles.Add(TopRightWrap);
		Triangles.Add(BottomRightWrap);

		Triangles.Add(TopLeftWrap);
		Triangles.Add(BottomRightWrap);
		Triangles.Add(TopRightWrap);
	}

	VerticalRail->CreateMeshSection_LinearColor(Segments++, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}

void AVerticalRailActor::GenerateCone(FVector Dimension, FVector Offset)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;

	int RingCount = 50;
	int PointsCount = 50;
	float Radius = Dimension.X/2;
	float Height_ = Dimension.X*2;

	for (int32 RingIndex = 0; RingIndex < RingCount; ++RingIndex) {
		for (int32 PointIndex = 0; PointIndex < PointsCount; ++PointIndex) {

			float Phi = 2 * PI * PointIndex;
			if (PointsCount > 1) {
				Phi /= (PointsCount - 1);
			}
			float SinPhi = FMath::Sin(Phi);
			float CosPhi = FMath::Cos(Phi);

			float U = Phi / (2 * PI);
			float V = RingIndex / RingCount;

			float DynamicRadius = Radius * (RingIndex / (RingCount - 1));

			FVector Vertex = FVector(DynamicRadius * SinPhi,  DynamicRadius * CosPhi, Height_ * (1 - (RingIndex / (RingCount - 1))));

			Vertices.Add(Vertex + Offset);
			UVs.Add(FVector2D{ Phi / (2 * PI),  (1.0 - (RingIndex / (RingCount - 1))) });

			if (RingIndex < RingCount - 1 && PointIndex < PointsCount - 1) {
				int32 CurrentVertexIndex = RingIndex * PointsCount + PointIndex;
				int32 NextVertexIndex = CurrentVertexIndex + PointsCount;

				Triangles.Add(CurrentVertexIndex);
				Triangles.Add(NextVertexIndex);
				Triangles.Add(NextVertexIndex + 1);
				//AddTriangle(CurrentVertexIndex, NextVertexIndex, NextVertexIndex + 1);

				Triangles.Add(NextVertexIndex + 1);
				Triangles.Add(CurrentVertexIndex + 1);
				Triangles.Add(CurrentVertexIndex);
				//AddTriangle(NextVertexIndex + 1, CurrentVertexIndex + 1, CurrentVertexIndex);
			}
		}
	}

	for (int32 RingIndex = 0; RingIndex < RingCount; ++RingIndex) {
		for (int32 PointIndex = 0; PointIndex < PointsCount; ++PointIndex) {
			if (RingIndex < RingCount - 1 && PointIndex < PointsCount - 1) {
				int32 CurrentVertexIndex = RingIndex * PointsCount + PointIndex;
				int32 NextVertexIndex = CurrentVertexIndex + PointsCount;

				FVector Normal = FVector::CrossProduct((Vertices[NextVertexIndex] - Vertices[CurrentVertexIndex]), (Vertices[NextVertexIndex + 1] - Vertices[NextVertexIndex])).GetSafeNormal();
				//Normals.Add(Normal);
			}
		}
	}

	for (int i = 0; i < Vertices.Num(); i++) {
		Normals.Add(FVector::ZeroVector);
	}

	for (int i = 0; i < Triangles.Num(); i += 3) {

		if (i + 2 < Triangles.Num()) {

			FVector vec1 = Vertices[Triangles[i + 1]] - Vertices[Triangles[i]];
			FVector vec2 = Vertices[Triangles[i + 2]] - Vertices[Triangles[i]];

			auto Normal = FVector::CrossProduct(vec2, vec1).GetSafeNormal();

			Normals[Triangles[i]] += Normal;
			Normals[Triangles[i + 1]] += Normal;
			Normals[Triangles[i + 2]] += Normal;

		}
	}


	for (int i = 0; i < Normals.Num(); i++) {
		Normals[i].Normalize();
	}
	
	VerticalRail->CreateMeshSection(Segments++, Vertices, Triangles, Normals, UVs, TArray<FColor>(), Tangents, true);
	//VerticalRail->SetMaterial(Segments++, Material);
}

void AVerticalRailActor::GenerateCircle(int Points , float Radius)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;

	// For Circle

	Vertices.Add(FVector(0, 0, 0));
	UVs.Add(FVector2D(0.5f, 0.5f));
	for (int i = 0; i <= Points; i++)
	{
		float Theta = i * 2 * PI / (Points -1);
		float SinTheta = FMath::Sin(Theta);
		float CosTheta = FMath::Cos(Theta);

		Vertices.Add(FVector(CosTheta, SinTheta, 0) * Radius);
		Normals.Add(FVector(0, 0, 1));

		
		UVs.Add(FVector2D(CosTheta / Radius / 2.0f + 0.5f, SinTheta / Radius / 2.0f + 0.5f));
	}

	for (int i = 1; i < Points; i++) {
		Triangles.Add(0);
		Triangles.Add(i + 1);
		Triangles.Add(i);
			
		Triangles.Add(0);
		Triangles.Add(i);
		Triangles.Add(i + 1);
	}

	// For Ring
	/*for (int i = 0; i <= Points; i++)
	{
		float Theta = i * 2 *PI / ((Points) - 1);
		float SinTheta = FMath::Sin(Theta);
		float CosTheta = FMath::Cos(Theta);


		Vertices.Add(FVector(CosTheta, SinTheta, 0) * Radius);

		Normals.Add(FVector(0,0, 1));
		UVs.Add(FVector2D(i / (Points ), 0));
	}

	Radius /= 2;
	for (int i = 0; i <=Points; i++)
	{
		float Theta = i * 2 * PI / (Points-1);
		float SinTheta = FMath::Sin(Theta);
		float CosTheta = FMath::Cos(Theta);

		Vertices.Add(FVector(CosTheta, SinTheta, 0) * Radius);
		Normals.Add(FVector(0,0, 1));
		UVs.Add(FVector2D(i / (Points), 1));
	}

	for (int i = 1; i < Points; i++) {

		int current = i;
		int next = current + Points;

		Triangles.Add(current);
		Triangles.Add(next);
		Triangles.Add(current + 1);

		Triangles.Add(current + 1);
		Triangles.Add(next);
		Triangles.Add(next + 1);
	}*/

	VerticalRail->CreateMeshSection(0, Vertices, Triangles, Normals, TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	VerticalRail->SetMaterial(0, Material_);
}

void AVerticalRailActor::GenerateCylinder(int Points, float Radius, float Height_)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;

	Vertices.Add(FVector(0, 0, 0));
	UVs.Add(FVector2D(0.5f, 0.5f));
	for (int i = 0; i <= Points; i++)
	{
		float Theta = i * 2 * PI / (Points - 1);
		float SinTheta = FMath::Sin(Theta);
		float CosTheta = FMath::Cos(Theta);

		Vertices.Add(FVector(CosTheta, SinTheta, 0) * Radius);

		UVs.Add(FVector2D(((CosTheta)+0.5f), -(SinTheta)+0.5f));
	}

	for (int i = 1; i < Points; i++) {
		Triangles.Add(0);
		Triangles.Add(i);
		Triangles.Add(i + 1);
	}

	Vertices.Add(FVector(0, 0, Height_));
	UVs.Add(FVector2D(0.5f, 0.5f));
	for (int i = 0; i <= Points; i++)
	{
		float Theta = i * 2 * PI / (Points - 1);
		float SinTheta = FMath::Sin(Theta);
		float CosTheta = FMath::Cos(Theta);

		Vertices.Add(FVector(CosTheta * Radius, SinTheta * Radius, Height_));
		UVs.Add(FVector2D(((-CosTheta)+0.5f), -(SinTheta)+0.5f));

	}

	for (int i = 0; i < (Points); i++) {
		Triangles.Add(Points+2);
		Triangles.Add(Points + 2+i + 1);
		Triangles.Add(Points + 2+i);
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < Points; j++) {
			float Theta = j * PI * 2 / (Points - 1);
			float SinTheta = FMath::Sin(Theta);
			float CosTheta = FMath::Cos(Theta);

			FVector vertex = FVector(CosTheta * Radius, SinTheta * Radius, Height_ * (1-((float)i / (10 - 1))));
			Vertices.Add(vertex);
			UVs.Add(FVector2D(-Theta / (2 * PI), ( ((float)i / (10 - 1)))));
			if (i < 10-1 && j < Points-1) {
				int current = ((2 * Points) + 4) + ((i * Points) + j);
				int next =  (current + Points);

				Triangles.Add(current);
				Triangles.Add(current + 1);
				Triangles.Add(next);

				Triangles.Add(next);
				Triangles.Add(current + 1);
				Triangles.Add(next + 1);
			}
		}
	}

	for (int i = 0; i < Vertices.Num(); i++) {
		Normals.Add(FVector::ZeroVector);
	}
	
	for (int i = 0; i < Triangles.Num(); i+=3) {

		if (i + 2 < Triangles.Num()) {

			FVector vec1 = Vertices[Triangles[i + 1]] - Vertices[Triangles[i]];
			FVector vec2 = Vertices[Triangles[i + 2]] - Vertices[Triangles[i]];

			auto Normal = FVector::CrossProduct(vec2, vec1).GetSafeNormal();

			Normals[Triangles[i]] += Normal;
			Normals[Triangles[i + 1]] += Normal;
			Normals[Triangles[i + 2]] += Normal;

		}
	}
	

	for (int i = 0; i < Normals.Num(); i++) {
		Normals[i].Normalize();
	}

	VerticalRail->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	VerticalRail->SetMaterial(0, Material_);
}





void AVerticalRailActor::GenerateVerticleRail(FVector Dimension)
{
	GenerateCube(Dimension);
	GenerateCube(FVector(Dimension.Z, Dimension.Y / 4, 15), FVector(0, -Dimension.X / 2 - Dimension.Y / 8, Dimension.Z / 4));
	GenerateCube(FVector(Dimension.Z, Dimension.Y / 4, 15), FVector(0, -Dimension.X / 2 - Dimension.Y / 8, -Dimension.Z / 4));
}

void AVerticalRailActor::RoundTurnedCapital(FVector Dimension) {

	GenerateVerticleRail(Dimension);
	GenerateBellShape(FVector(0, 0, Dimension.Z / 2), Dimension.X / 2, Dimension.X / 3, Dimension.X / 4, 1, 10, 10);
	GenerateSphere(Dimension, FVector(0, 0, (Dimension.X / 3) + (Dimension.Z / 2) + SphereRadius));
}

void AVerticalRailActor::AcornCapital(FVector Dimension)
{
	GenerateVerticleRail(Dimension);
	GenerateBellShape(FVector(0, 0, Dimension.Z / 2), Dimension.X / 2, Dimension.X / 3, Dimension.X / 4, 1, 10, 10);
	GenerateEgg(Dimension , FVector(0, 0, Dimension.X / 3 + (Dimension.Z / 2) + (Dimension.X)));
}

void AVerticalRailActor::PyramidTop(FVector Dimension)
{
	GenerateVerticleRail(Dimension);
	GenerateCube(FVector(Dimension.X - 5, Dimension.Y - 5, 3), FVector(0, 0, (Dimension.Z/2) + 1.5));
	GenerateCube(FVector(Dimension.X, Dimension.X, Dimension.X), FVector(0, 0, (Dimension.Z / 2) + (Dimension.X / 2) + 3));
	GeneratePyramid(Dimension, FVector(0, 0, (Dimension.X + (Dimension.Z / 2) + 3)));
}

void AVerticalRailActor::GothicStarTop(FVector Dimension)
{
	GenerateVerticleRail(Dimension);
	GenerateCube(FVector(Dimension.X - 5, Dimension.Y - 5, 3), FVector(0, 0, (Dimension.Z / 2) + 1.5));
	GenerateCube(FVector(Dimension.X, Dimension.X, Dimension.X), FVector(0, 0, (Dimension.Z / 2) + (Dimension.X / 2) + 3));
	GenerateCone(Dimension, FVector(0, 0, (Dimension.Z / 2) + Dimension.X + 3));
}

void AVerticalRailActor::SetVerticalMaterial(UMaterialInterface* Material)
{
	VerticalRail->SetMaterial(0 , Material);
}

void AVerticalRailActor::SetHorizontalMaterial(UMaterialInterface* Material)
{
	VerticalRail->SetMaterial(1, Material);
	VerticalRail->SetMaterial(2, Material);

}

void AVerticalRailActor::SetTopMaterial(UMaterialInterface* Material) {
	VerticalRail->SetMaterial(3, Material);
	VerticalRail->SetMaterial(4, Material);
	VerticalRail->SetMaterial(5, Material);
}

void AVerticalRailActor::RoundedOverTop(FVector Dimension)
{
	GenerateVerticleRail(Dimension);
	GenerateCube(FVector(Dimension.X - 5, Dimension.Y - 5, 3), FVector(0, 0, (Dimension.Z / 2) + 1.5));
	GenerateCube(FVector(Dimension.X, Dimension.X, Dimension.X), FVector(0, 0, (Dimension.Z / 2) + (Dimension.X / 2) + 3));
	GenerateCapsule(Dimension, FVector(0, 0, (Dimension.X) + 3+ (Dimension.Z / 2)));
}

void AVerticalRailActor::WindsorTurnedCapital(FVector Dimension)
{
	GenerateVerticleRail(Dimension);
	GenerateBellShape(FVector(0, 0, Dimension.Z / 2), Dimension.X / 2, Dimension.X / 3, Dimension.X / 4, 1, 10, 10);
	GenerateSphere(Dimension, FVector(0, 0, (Dimension.X/3) + (Dimension.Z / 2) + SphereRadius));
	GenerateBellShape(FVector(0, 0, (Dimension.Z / 2) + (Dimension.X/3) + (SphereRadius * 2)), Dimension.X / 8, Dimension.X / 4, Dimension.X / 40, 1, 10, 10);
}

void AVerticalRailActor::Generate(FVector Dimension)
{
	Segments = 0;

	switch (TopType) {
	case ETopType::RoundTurnedCapital:
		VerticalRail->ClearAllMeshSections();
		RoundTurnedCapital(Dimension);
		break;

	case ETopType::PyramidTop:
		VerticalRail->ClearAllMeshSections();
		PyramidTop(Dimension);
		break;

	case ETopType::AcornCapital:
		VerticalRail->ClearAllMeshSections();
		AcornCapital(Dimension);
		break;

	case ETopType::RoundedOverTop:
		VerticalRail->ClearAllMeshSections();
		RoundedOverTop(Dimension);
		break;

	case ETopType::WindsorTurnedCapital:
		VerticalRail->ClearAllMeshSections();
		WindsorTurnedCapital(Dimension);
		break;

	case ETopType::GothicStartTop:
		VerticalRail->ClearAllMeshSections();
		GothicStarTop(Dimension);
		break;

	case ETopType::VerticleRail:
		VerticalRail->ClearAllMeshSections();
		GenerateVerticleRail(Dimension);
		break;
	}

}
