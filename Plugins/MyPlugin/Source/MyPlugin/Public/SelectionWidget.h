// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MeshSelectionScrollBox.h"
#include <Components/CanvasPanel.h>
#include "SelectionWidget.generated.h"


UCLASS()
class MYPLUGIN_API USelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Canvas;
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite  , meta = (BindWidget))
	UMeshSelectionScrollBox* MeshSelectionScrollBox;
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite  , meta = (BindWidget))
	UMeshSelectionScrollBox* MaterialSelectionScrollBox;
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite  , meta = (BindWidget))
	UMeshSelectionScrollBox* TextureSelectionScrollBox;

	virtual void NativeConstruct() override;
};
