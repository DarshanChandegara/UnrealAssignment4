#include "FenceMeshActor.h"

AFenceMeshActor::AFenceMeshActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(Scene);

}

void AFenceMeshActor::BeginPlay()
{
	Super::BeginPlay();
	ReplaceStaticMeshWithProceduralMesh();
}

void AFenceMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFenceMeshActor::OnConstruction(const FTransform& Transform)
{
	//DestroyRails();
	//GenerateRails();
	ClearStaticRails();
	AddStaticRails();
	UE_LOG(LogTemp, Warning, TEXT("On Construction"));

}

void AFenceMeshActor::GenerateRails()
{

	GenerateSplinePoints();

	Spline->SetSplinePoints(points , ESplineCoordinateSpace::Local);

	if (VerticalRailActorClass) {

		for (int i = 0; i < points.Num(); i++) {
			auto CurrentLocaton = Spline->GetLocationAtSplinePoint(i , ESplineCoordinateSpace::Local);
			auto CurrentRotation = Spline->GetRotationAtSplinePoint(i ,ESplineCoordinateSpace::Local);

			CurrentLocaton.Z = CurrentLocaton.Z + (FenceProperties.Height / 2);

			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AVerticalRailActor* SpawnActor =  GetWorld()->SpawnActor<AVerticalRailActor>(VerticalRailActorClass, CurrentLocaton, CurrentRotation, params);

			if (SpawnActor) {
				SpawnActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				//SpawnActor->Generate(FVector(FenceProperties.Width ,FenceProperties.Width , FenceProperties.Height));
				SpawnActor->Generate(FVector(25,25,250));
				RailArray.Add(SpawnActor);
			}
		}
	}
}

void AFenceMeshActor::DestroyRails()
{
	for (int i = 0; i < RailArray.Num(); i++) {
		if (RailArray[i]) {
			RailArray[i]->Destroy();
			RailArray[i] = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("Inside Destroy"));
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Inside Destroy 2"));

	RailArray.Empty();
	Spline->ClearSplinePoints();
	points.Empty();
}

void AFenceMeshActor::GenerateSplinePoints()
{
	float LengthCovered = 0;
	while (LengthCovered <= FenceProperties.Length * 100) {
		FVector location = FVector(LengthCovered, 0, 0);
		points.Add(location);

		if (FenceProperties.Spacing > FenceProperties.Height) {
			FenceProperties.Spacing = FenceProperties.Height;
		}
		LengthCovered += FenceProperties.Spacing;
	}
}

void AFenceMeshActor::AddStaticRails()
{
	ClearStaticRails();
	auto SplineLength = Spline->GetSplineLength();
	auto MeshLength = FenceProperties.Spacing ;
	auto NoOfRails = SplineLength / MeshLength;

	for (int i = 0; i < Spline->GetNumberOfSplinePoints(); i++) {
		Spline->SetSplinePointType(i , ESplinePointType::Linear);
	}

	for (int i = 0; i < NoOfRails; i++) {
		FVector location = Spline->GetLocationAtDistanceAlongSpline(i*MeshLength , ESplineCoordinateSpace::World);
		location.Z += (250 * FenceProperties.Height / 10) / 2;
		FRotator rotation = Spline->GetRotationAtDistanceAlongSpline(i*MeshLength , ESplineCoordinateSpace::World);

		UStaticMeshComponent* VerticleRail = NewObject<UStaticMeshComponent>(this);
		VerticleRail->AttachToComponent(Spline , FAttachmentTransformRules::KeepRelativeTransform);
		VerticleRail->SetWorldLocationAndRotation(location , rotation); 
		VerticleRail->SetStaticMesh(StaticMesh);
		VerticleRail->SetRelativeScale3D(FVector(FenceProperties.Length / 10, FenceProperties.Width / 10, FenceProperties.Height / 10));
		VerticleRail->RegisterComponent();
		StaticMeshRailArray.Add(VerticleRail);
	}
}

void AFenceMeshActor::ClearStaticRails()
{
	for (auto& MeshComponent : StaticMeshRailArray)
	{
		if (IsValid(MeshComponent))
			MeshComponent->DestroyComponent();
	}
	StaticMeshRailArray.Empty();
}

void AFenceMeshActor::ReplaceStaticMeshWithProceduralMesh()
{
	ClearStaticRails();
	if (VerticalRailActorClass) {

		auto SplineLength = Spline->GetSplineLength();
		auto MeshLength = FenceProperties.Spacing;
		auto NoOfRails = SplineLength / MeshLength;

		for (int i = 0; i < NoOfRails; i++) {
			FVector location = Spline->GetLocationAtDistanceAlongSpline(i * MeshLength, ESplineCoordinateSpace::World);
			location.Z += (250 * FenceProperties.Height/10)/2;
			FRotator rotation = Spline->GetRotationAtDistanceAlongSpline(i * MeshLength, ESplineCoordinateSpace::World);
			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AVerticalRailActor* SpawnActor = GetWorld()->SpawnActor<AVerticalRailActor>(VerticalRailActorClass, location, rotation, params);

			if (SpawnActor) {
				SpawnActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				SpawnActor->Generate(FVector(25,25,250));
				SpawnActor->SetActorRelativeScale3D(FVector(FenceProperties.Length / 10, FenceProperties.Width / 10, FenceProperties.Height / 10));

				UMaterialInstanceDynamic* VerticalMaterialInstance = UMaterialInstanceDynamic::Create(Material, this);

				TileX = FenceProperties.Length / 20 ;
				TileY = FenceProperties.Height / 2;

				VerticalMaterialInstance->SetScalarParameterValue(TEXT("TileX"), TileX);
				VerticalMaterialInstance->SetScalarParameterValue(TEXT("TileY"), TileY);

				SpawnActor->SetVerticalMaterial(VerticalMaterialInstance);
				
				
				UMaterialInstanceDynamic* HorizontaMaterialInstance = UMaterialInstanceDynamic::Create(Material, this);

				TileX = FenceProperties.Length / 2; 
				TileY = FenceProperties.Height / 20;

				HorizontaMaterialInstance->SetScalarParameterValue(TEXT("TileX"), TileX);
				HorizontaMaterialInstance->SetScalarParameterValue(TEXT("TileY"), TileY);

				SpawnActor->SetHorizontalMaterial(HorizontaMaterialInstance);
				SpawnActor->SetTopMaterial(Material);

			}
		}
	}
}

