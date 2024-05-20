// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SMeshSelectionScrollBox.h"
#include "MeshSelectionScrollBox.generated.h"


DECLARE_DELEGATE_OneParam(FOnMeshSelectEvent, const FMeshData&)
DECLARE_DELEGATE_OneParam(FOnMaterialSelectEvent, const FMaterialData&)
DECLARE_DELEGATE_OneParam(FOnTextureSelectEvent, const FTextureData&)

UCLASS()
class MYPLUGIN_API UMeshSelectionScrollBox : public UWidget
{
	GENERATED_BODY()

public:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual const FText GetPaletteCategory() override;
	virtual void ReleaseSlateResources(bool breleaseresource) override;

	TSharedPtr<SMeshSelectionScrollBox> ScrollBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EAssetType> AssetType;
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	UMeshAssetManager* MeshAssetManager;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	int ImageSize;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	int HeadTextSize;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	FString HeadText;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	int ThumbnailTextSize;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	FSlateColor BGColor;



	void HandleOnMeshSelected(const FMeshData& MeshData);
	void HandleOnMaterialSelected(const FMaterialData& MeshData);
	void HandleOnTextureSelected(const FTextureData& MeshData);

	FOnMeshSelectEvent OnMeshSelectEvent;

	FOnMaterialSelectEvent OnMaterialSelectEvent;

	FOnTextureSelectEvent OnTextureSelectEvent;
	
};
