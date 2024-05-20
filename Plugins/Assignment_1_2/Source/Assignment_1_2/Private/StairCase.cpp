// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/StairCase.h"

// Sets default values
AStairCase::AStairCase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	myScene = CreateDefaultSubobject<USceneComponent>("MyDefault");
	myScene->SetRelativeLocation(FVector(0, 0, 0));
	Distance = 1;
	RootComponent = myScene; 
	length = 1;
	width = 2;
	height = 0.5;
	NoOfSteps = 10; 
	locationOffset = FVector(0,0,0);
	isRailAttach = false;
	}

// Called when the game starts or when spawned
void AStairCase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStairCase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStairCase::OnConstruction(const FTransform& transform) 
{

	createStairs();
}

void AStairCase::createStairs() {

	if (stairsEnum == StairEnum::BoxStair) createBoxStair();
	else if (stairsEnum == StairEnum::CloseStair) createCloseStair();
	else createOpenStair();
}

void AStairCase::createOpenStair()
{
	for (int i = 0; i < meshArray.Num(); i++) {
		if (meshArray[i]) {
			meshArray[i]->DestroyComponent();
		}
	}

	for (int i = 0; i < leftRailArray.Num(); i++) {
		if (leftRailArray[i]) {
			leftRailArray[i]->DestroyComponent();
		}
	}

	for (int i = 0; i < rightRailArray.Num(); i++) {
		if (rightRailArray[i]) {
			rightRailArray[i]->DestroyComponent();
		}
	}

	for (int i = 0; i < leftPlainArray.Num(); i++) {
		if (leftPlainArray[i]) {
			leftPlainArray[i]->DestroyComponent();
		}
	}

	for (int i = 0; i < rightPlainArray.Num(); i++) {
		if (rightPlainArray[i]) {
			rightPlainArray[i]->DestroyComponent();
		}
	}

	meshArray.Empty();
	leftRailArray.Empty();
	rightRailArray.Empty();
	leftPlainArray.Empty();
	rightPlainArray.Empty();

	for (int i = 0; i < NoOfSteps; i++) {

		UE_LOG(LogTemp, Warning, TEXT("This is onConstruction + %d"), i);
		//FString name = "Mesh" + FString::FromInt(i);
		UStaticMeshComponent* stairs = NewObject<UStaticMeshComponent>(this);
		stairs->AttachToComponent(myScene, FAttachmentTransformRules::KeepRelativeTransform);
		stairs->RegisterComponentWithWorld(GetWorld());
		FVector location; 
		if (i == 0) {
			location = FVector(length * 100, 0, height * 100); 
		}
		else {
			location = FVector(meshArray[i - 1]->GetRelativeScale3D().X * 100 * (i + 1), 0, meshArray[i - 1]->GetRelativeScale3D().Z * 100 * (i + 1) + (Distance * 100 * i)/2);
		}
		FVector relativeLocation = location;

		stairs->SetRelativeScale3D(FVector(length, width, height)); 
		stairs->SetRelativeLocation(relativeLocation);
		stairs->SetStaticMesh(sourceMeshStairs);
		meshArray.Add(stairs);


		if (isRailAttach) {

			UStaticMeshComponent* leftRail = NewObject<UStaticMeshComponent>(this);
			leftRail->AttachToComponent(stairs, FAttachmentTransformRules::KeepRelativeTransform);
			leftRail->RegisterComponentWithWorld(GetWorld());
			leftRail->SetRelativeLocation(FVector(0, -50, 0));
			leftRail->SetRelativeScale3D(FVector(2.0, 0.05, 1));
			leftRail->SetStaticMesh(sourceMeshRails);
			leftRailArray.Add(leftRail); 

			UStaticMeshComponent* rightRail = NewObject<UStaticMeshComponent>(this);
			rightRail->AttachToComponent(stairs, FAttachmentTransformRules::KeepRelativeTransform);
			rightRail->RegisterComponentWithWorld(GetWorld());
			rightRail->SetRelativeLocation(FVector(0, 50, 0));
			rightRail->SetRelativeScale3D(FVector(2.0, 0.05, 1));
			rightRail->SetStaticMesh(sourceMeshRails);
			rightRailArray.Add(rightRail);

			if (i != NoOfSteps - 1) {

				UStaticMeshComponent* leftPlain = NewObject<UStaticMeshComponent>(this);
				leftPlain->AttachToComponent(stairs, FAttachmentTransformRules::KeepRelativeTransform);
				leftPlain->RegisterComponentWithWorld(GetWorld());

				leftPlain->SetRelativeLocation(FVector(-0, -50, 500));
				leftPlain->SetWorldRotation(FRotator(180, 0, 180));
				leftPlain->SetRelativeScale3D(FVector(1, 0.025, 1.5 + Distance/4));
				leftPlain->SetStaticMesh(sourceMeshPlain);
				leftPlainArray.Add(leftPlain);

				UStaticMeshComponent* rightPlain = NewObject<UStaticMeshComponent>(this);
				rightPlain->AttachToComponent(stairs, FAttachmentTransformRules::KeepRelativeTransform);
				rightPlain->RegisterComponentWithWorld(GetWorld());

				rightPlain->SetRelativeLocation(FVector(-0, 50, 500));
				rightPlain->SetWorldRotation(FRotator(180, 0, 180));
				rightPlain->SetRelativeScale3D(FVector(1, 0.025, 1.5 + Distance/4));
				rightPlain->SetStaticMesh(sourceMeshPlain);
				rightPlainArray.Add(rightPlain);
			}

			
		}
	}
}

void AStairCase::createCloseStair() {
	for (int i = 0; i < meshArray.Num(); i++) {
		if (meshArray[i]) {
			meshArray[i]->DestroyComponent();
		}
	}

	for (int i = 0; i < leftRailArray.Num(); i++) {
		if (leftRailArray[i]) {
			leftRailArray[i]->DestroyComponent();
		}
	}

	for (int i = 0; i < rightRailArray.Num(); i++) {
		if (rightRailArray[i]) {
			rightRailArray[i]->DestroyComponent();
		}
	}

	for (int i = 0; i < leftPlainArray.Num(); i++) {
		if (leftPlainArray[i]) {
			leftPlainArray[i]->DestroyComponent();
		}
	}

	for (int i = 0; i < rightPlainArray.Num(); i++) {
		if (rightPlainArray[i]) {
			rightPlainArray[i]->DestroyComponent();
		}
	}

	meshArray.Empty();
	leftRailArray.Empty();
	rightRailArray.Empty();
	leftPlainArray.Empty();
	rightPlainArray.Empty();

	for (int i = 0; i < NoOfSteps; i++) {

		UE_LOG(LogTemp, Warning, TEXT("This is onConstruction + %d"), i);

		UStaticMeshComponent* stairs = NewObject<UStaticMeshComponent>(this);
		stairs->AttachToComponent(myScene, FAttachmentTransformRules::KeepRelativeTransform);
		stairs->RegisterComponentWithWorld(GetWorld());
		FVector location;
		if (i == 0) {
			location = FVector(length * 100, 0, height * 100);
		}
		else {
			location = FVector(meshArray[i - 1]->GetRelativeScale3D().X * 100 * (i + 1), 0, meshArray[i - 1]->GetRelativeScale3D().Z * 100 * (i + 1));
		}
		FVector relativeLocation = location;

		stairs->SetRelativeScale3D(FVector(length, width, height));



		stairs->SetRelativeLocation(relativeLocation);
		stairs->SetStaticMesh(sourceMeshStairs);
		meshArray.Add(stairs);

		if (isRailAttach) {

			UStaticMeshComponent* leftRail = NewObject<UStaticMeshComponent>(this);
			leftRail->AttachToComponent(stairs, FAttachmentTransformRules::KeepRelativeTransform);
			leftRail->RegisterComponentWithWorld(GetWorld());
			leftRail->SetRelativeLocation(FVector(0, -50, 0));
			leftRail->SetRelativeScale3D(FVector(2.0, 0.05, 1.5));
			leftRail->SetStaticMesh(sourceMeshRails);

			UStaticMeshComponent* rightRail = NewObject<UStaticMeshComponent>(this);
			rightRail->AttachToComponent(stairs, FAttachmentTransformRules::KeepRelativeTransform);
			rightRail->RegisterComponentWithWorld(GetWorld());
			rightRail->SetRelativeLocation(FVector(0, 50, 0));

			rightRail->SetRelativeScale3D(FVector(2.0, 0.05, 1.5));
			rightRail->SetStaticMesh(sourceMeshRails);

			if (i != NoOfSteps - 1) {

				UStaticMeshComponent* leftPlain = NewObject<UStaticMeshComponent>(this);
				leftPlain->AttachToComponent(stairs, FAttachmentTransformRules::KeepRelativeTransform);
				leftPlain->RegisterComponentWithWorld(GetWorld());

				leftPlain->SetRelativeLocation(FVector(0, -50, 750));


				//leftPlain->SetRelativeLocation(FVector(0, -50, (leftPlain->GetComponentLocation().Z)*0.68));
				leftPlain->SetWorldRotation(FRotator(180, 0, 180));
				leftPlain->SetRelativeScale3D(FVector(1, 0.025, 1));
				leftPlain->SetStaticMesh(sourceMeshPlain);
				leftPlainArray.Add(leftPlain);

				UStaticMeshComponent* rightPlain = NewObject<UStaticMeshComponent>(this);
				rightPlain->AttachToComponent(stairs, FAttachmentTransformRules::KeepRelativeTransform);
				rightPlain->RegisterComponentWithWorld(GetWorld());

				rightPlain->SetRelativeLocation(FVector(0, 50, 750));


				//leftPlain->SetRelativeLocation(FVector(0, -50, (leftPlain->GetComponentLocation().Z)*0.68));
				rightPlain->SetWorldRotation(FRotator(180, 0, 180));
				rightPlain->SetRelativeScale3D(FVector(1, 0.025, 1));
				rightPlain->SetStaticMesh(sourceMeshPlain);
				rightPlainArray.Add(rightPlain);
			}




			leftRailArray.Add(leftRail);
			rightRailArray.Add(rightRail);
		}
	}
}

void AStairCase::createBoxStair() {
	for (int i = 0; i < meshArray.Num(); i++) {
		if (meshArray[i]) {
			meshArray[i]->DestroyComponent();
		}
	}

	for (int i = 0; i < leftRailArray.Num(); i++) {
		if (leftRailArray[i]) {
			leftRailArray[i]->DestroyComponent();
		}
	}

	for (int i = 0; i < rightRailArray.Num(); i++) {
		if (rightRailArray[i]) {
			rightRailArray[i]->DestroyComponent();
		}
	}

	for (int i = 0; i < leftPlainArray.Num(); i++) {
		if (leftPlainArray[i]) {
			leftPlainArray[i]->DestroyComponent();
		}
	}

	for (int i = 0; i < rightPlainArray.Num(); i++) {
		if (rightPlainArray[i]) {
			rightPlainArray[i]->DestroyComponent();
		}
	}

	meshArray.Empty();
	leftRailArray.Empty();
	rightRailArray.Empty();
	leftPlainArray.Empty();
	rightPlainArray.Empty();

	for (int i = 0; i < NoOfSteps; i++) {

		UE_LOG(LogTemp, Warning, TEXT("This is onConstruction + %d"), i);
		//FString name = "Mesh" + FString::FromInt(i);
		UStaticMeshComponent* stairs = NewObject<UStaticMeshComponent>(this);
		stairs->AttachToComponent(myScene, FAttachmentTransformRules::KeepRelativeTransform);
		stairs->RegisterComponentWithWorld(GetWorld());
		FVector location;
		if (i == 0) {
			location = FVector(length * 100, 0, 0);
			stairs->SetRelativeScale3D(FVector(length, width, height));
		}
		else {
			location = FVector(meshArray[i - 1]->GetRelativeScale3D().X * 100 * (i + 1), 0, 0);
			stairs->SetRelativeScale3D(FVector(length, width, meshArray[i - 1]->GetRelativeScale3D().Z + 1));
		}
		FVector relativeLocation = location;

		stairs->SetRelativeLocation(relativeLocation);
		stairs->SetStaticMesh(sourceMeshStairs);
		meshArray.Add(stairs);

		if (isRailAttach) {
			UStaticMeshComponent* leftRail = NewObject<UStaticMeshComponent>(this);
			leftRail->AttachToComponent(myScene, FAttachmentTransformRules::KeepRelativeTransform);
			leftRail->RegisterComponentWithWorld(GetWorld());
			leftRail->SetRelativeLocation(FVector(meshArray[i]->GetRelativeScale3D().X * 100 * (i + 1), -width * 50, meshArray[i]->GetRelativeScale3D().Z * 100));
			leftRail->SetRelativeScale3D(FVector(2 * length, 0.05, 1));
			leftRail->SetStaticMesh(sourceMeshRails);
			leftRailArray.Add(leftRail);

			UStaticMeshComponent* rightRail = NewObject<UStaticMeshComponent>(this);
			rightRail->AttachToComponent(myScene, FAttachmentTransformRules::KeepRelativeTransform);
			rightRail->RegisterComponentWithWorld(GetWorld());
			rightRail->SetRelativeLocation(FVector(meshArray[i]->GetRelativeScale3D().X * 100 * (i + 1), width * 50, meshArray[i]->GetRelativeScale3D().Z * 100));
			rightRail->SetRelativeScale3D(FVector(2 * length, 0.05, 1));
			rightRail->SetStaticMesh(sourceMeshRails);
			rightRailArray.Add(rightRail);

			if (i != NoOfSteps - 1) {

				UStaticMeshComponent* leftPlain = NewObject<UStaticMeshComponent>(this);
				leftPlain->AttachToComponent(myScene, FAttachmentTransformRules::KeepRelativeTransform);
				leftPlain->RegisterComponentWithWorld(GetWorld());

				leftPlain->SetRelativeLocation(FVector(meshArray[i]->GetRelativeScale3D().X * 100 * (i + 1), -width * 50, (meshArray[i]->GetRelativeScale3D().Z + leftRail->GetRelativeScale3D().Z * 5) * 100));
				leftPlain->SetWorldRotation(FRotator(180, 0, 180));
				leftPlain->SetRelativeScale3D(FVector(length, 0.025, 1));
				leftPlain->SetStaticMesh(sourceMeshPlain);
				leftPlainArray.Add(leftPlain);

				UStaticMeshComponent* rightPlain = NewObject<UStaticMeshComponent>(this);
				rightPlain->AttachToComponent(myScene, FAttachmentTransformRules::KeepRelativeTransform);
				rightPlain->RegisterComponentWithWorld(GetWorld());

				rightPlain->SetRelativeLocation(FVector(meshArray[i]->GetRelativeScale3D().X * 100 * (i + 1), width * 50, (meshArray[i]->GetRelativeScale3D().Z + rightRail->GetRelativeScale3D().Z * 5) * 100));
				rightPlain->SetWorldRotation(FRotator(180, 0, 180));
				rightPlain->SetRelativeScale3D(FVector(length, 0.025, 1));
				rightPlain->SetStaticMesh(sourceMeshPlain);
				rightPlainArray.Add(rightPlain);
			}
		}
	}
}


