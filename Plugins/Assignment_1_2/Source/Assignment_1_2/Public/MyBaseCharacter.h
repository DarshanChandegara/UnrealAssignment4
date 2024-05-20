// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyPawnAttributesDataAsset.h"
#include "MyBaseCharacter.generated.h"

UCLASS()
class ASSIGNMENT_1_2_API AMyBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyBaseCharacter();

	UPROPERTY(EditAnywhere)
	UMyPawnAttributesDataAsset* PawnAttributeAsset;
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
