// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPLUGIN_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = (BindWidget))
	UTextBlock* NotificationText;
	
};
