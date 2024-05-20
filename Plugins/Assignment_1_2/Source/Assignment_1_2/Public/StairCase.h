// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StairCase.generated.h"

UENUM()
enum class StairEnum {
	BoxStair,
	OpenStair,
	CloseStair
};

UCLASS()
class ASSIGNMENT_1_2_API AStairCase : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AStairCase();

protected:
	
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category="Stairs")
	float length;
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category="Stairs")
	float width;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category="Stairs")
	float height;
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category="Stairs")
	int NoOfSteps;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "ShowRails") , Category="Stairs")
	bool isRailAttach;

	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category="Stairs")
	TEnumAsByte<StairEnum> stairsEnum;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "stairEnum == StairEnum::OpenStair"))
	int Distance;

	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "Stairs")
	UStaticMesh * sourceMeshStairs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs")
	UStaticMesh* sourceMeshRails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs")
	UStaticMesh* sourceMeshPlain;  

	UPROPERTY()
	FVector locationOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySceneComponent")
	USceneComponent* myScene;  

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySceneComponent")  
	TArray<UStaticMeshComponent*> meshArray;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySceneComponent")  
	TArray<UStaticMeshComponent*> leftRailArray; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySceneComponent")
	TArray<UStaticMeshComponent*> leftPlainArray; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySceneComponent")  
	TArray<UStaticMeshComponent*> rightRailArray; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySceneComponent")
	TArray<UStaticMeshComponent*> rightPlainArray;

	virtual void Tick(float DeltaTime) override;

	void OnConstruction(const FTransform& trans) override; 

	void createOpenStair();

	void createCloseStair();

	void createBoxStair();

	UFUNCTION(BlueprintCallable)
	void createStairs();
};
