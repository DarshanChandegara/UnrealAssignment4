// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataTable.h"
#include "MyPawnAttributesDataAsset.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "MyPlayerController.generated.h"

UENUM()
enum class EPawnType : int8 {
	ThirdPerson,
	TopDown,
	FirstPerson
};

USTRUCT(BlueprintType)
struct FPawnDataTable : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EPawnType> PawnType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> Pawn;

};

UCLASS()
class ASSIGNMENT_1_2_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

	AMyPlayerController();

public:
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	UInputAction* possesAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	UInputMappingContext* mapping;

	UPROPERTY(EditAnywhere) 
	UDataTable* Table;

	UPROPERTY()
	APawn* currentPawn;

	UPROPERTY()
	int32 Index = 0;

	void spawn();

	UPROPERTY(EditAnywhere, category = "PAWN")
	TEnumAsByte<EPawnType> pawnType;

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayAttributes(UMyPawnAttributesDataAsset* asset); 

	void OnPossess(APawn* pawn) override;
	
};
