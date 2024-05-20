// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "TP_TopDownPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class ASSIGNMENT4_API ATP_TopDownPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATP_TopDownPlayerController();
};


