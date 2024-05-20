// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyPawnAttributesDataAsset.generated.h"

UCLASS()
class ASSIGNMENT_1_2_API UMyPawnAttributesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta=(ClampMin = 0 , ClampMax=100 , UIMin = 0 , UIMax = 100))
	float Health;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta =(ClampMin= 0 , UIMin = 0))
	float Speeed;
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = (ClampMin= 0, UIMin = 0))
	float JumpHeight;
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	FString Type;

	UFUNCTION(BlueprintCallable)
	float GetHealth() {
		return Health;
	}

	
	
};
