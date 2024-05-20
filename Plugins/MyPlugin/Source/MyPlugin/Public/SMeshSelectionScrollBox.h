// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeshAssetManager.h"
#include <Widgets/Layout/SScrollBox.h>
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SWidget.h"

UENUM()
enum class EAssetType :uint8 {
	Mesh , 
	Material ,
	Texture
};

DECLARE_DELEGATE_OneParam(FOnMeshSelected , const FMeshData& MeshData)
DECLARE_DELEGATE_OneParam(FOnMaterialSelected , const FMaterialData& MeshData)
DECLARE_DELEGATE_OneParam(FOnTextureSelected , const FTextureData& MeshData)

class MYPLUGIN_API SMeshSelectionScrollBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMeshSelectionScrollBox)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<UMeshAssetManager> , InMeshAssetManager)
	SLATE_ARGUMENT(TEnumAsByte<EAssetType> , InAssetType)
	SLATE_ARGUMENT(int , InImageSize)
	SLATE_ARGUMENT(int , InHeadTextSize)
	SLATE_ARGUMENT(FString , InHeadText)
	SLATE_ARGUMENT(int , InThumbnailTextSize)
	SLATE_ARGUMENT(FSlateColor  , InBGColor)
	SLATE_END_ARGS()


	TWeakObjectPtr<UMeshAssetManager> MeshAssetManager;
	TEnumAsByte<EAssetType> AssetType;
	int ImageSize;
	int HeadTextSize;
	FString HeadString;
	int ThumbnailTextSize;
	FSlateColor BGColor;


	TSharedPtr<SScrollBox> ScrollBox;
	TSharedPtr<SVerticalBox> RootVerticalBox;
	TSharedPtr<STextBlock> HeadText;


	FOnMeshSelected OnMeshSelected;
	FOnMaterialSelected OnMaterialSelected;
	FOnTextureSelected OnTextureSelected;


	void Construct(const FArguments& InArgs);
	void CreateScrollBox();
	void CreateMeshAssetScrollBox();
	void CreateMaterialScrollBox();
	void CreateTextureScrollBox();
};
