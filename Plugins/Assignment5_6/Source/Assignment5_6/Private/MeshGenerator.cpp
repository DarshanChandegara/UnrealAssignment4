// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshGenerator.h"

AMeshGenerator::AMeshGenerator()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMeshGenerator::BeginPlay()
{
	Super::BeginPlay();

}

void AMeshGenerator::FinishScatter()
{
	if (AsyncScatterTask)
	{
		if (!AsyncScatterTask->IsDone()) {
			AsyncScatterTask->EnsureCompletion();

		}
		delete AsyncScatterTask;
		AsyncScatterTask = nullptr;
	}
}

void AMeshGenerator::ScatterObjects()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::FromInt(HISMComponents.Num()));

	for (auto& Pair : HISMComponents)
	{
		if (UHierarchicalInstancedStaticMeshComponent* HISM = Pair.Value)
		{
			HISM->ClearInstances();
		}
	}


	FinishScatter();

	AsyncScatterTask = new FAsyncTask<FAsyncScatterTask>(this, Location, Dimension);
	AsyncScatterTask->StartBackgroundTask();
}

void AMeshGenerator::AddInstances(UStaticMesh* StaticMesh, const FTransform& Transform, UMaterialInterface* Material)
{
	AsyncTask(ENamedThreads::GameThread, [this, StaticMesh, Transform , Material]()
		{
			UHierarchicalInstancedStaticMeshComponent** HISMCPtr = HISMComponents.Find(StaticMesh);
			if (HISMCPtr && *HISMCPtr && (*HISMCPtr)->IsValidLowLevel())
			{
				(*HISMCPtr)->SetMaterial(0, Material);
				(*HISMCPtr)->AddInstance(Transform, false);
			}
			else
			{
				UHierarchicalInstancedStaticMeshComponent* NewHISMC = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);

				UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(0, Material);
				NewHISMC->SetStaticMesh(StaticMesh);
				if (Material) {
					//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, "Inside Material");
					NewHISMC->SetMaterial(0, Material);
				}
				HISMComponents.Add(StaticMesh, NewHISMC);

				NewHISMC->RegisterComponentWithWorld(GetWorld());
				NewHISMC->AddInstance(Transform, false);
			}

			UpdateEventPrgress.ExecuteIfBound(ProgressPrecent);
		});
}

void AMeshGenerator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//FinishScatter();
	Super::EndPlay(EndPlayReason);
}

void AMeshGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DataAsset)
	{
		UDataAsset* dAsset = DataAsset;

	}
}


