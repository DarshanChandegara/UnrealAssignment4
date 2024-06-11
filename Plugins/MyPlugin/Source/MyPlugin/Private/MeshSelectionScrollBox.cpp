// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshSelectionScrollBox.h"

TSharedRef<SWidget> UMeshSelectionScrollBox::RebuildWidget()
{
	ScrollBar = SNew(SMeshSelectionScrollBox).InMeshAssetManager(MeshAssetManager).InAssetType(AssetType).InImageSize(ImageSize).InHeadTextSize(HeadTextSize).InHeadText(HeadText).InThumbnailTextSize(ThumbnailTextSize).InBGColor(BGColor);
	if (ScrollBar) {
		ScrollBar->OnMeshSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleOnMeshSelected);
		ScrollBar->OnTextureSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleOnTextureSelected);
		ScrollBar->OnMaterialSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleOnMaterialSelected);
	}
	return ScrollBar.ToSharedRef();
}

const FText UMeshSelectionScrollBox::GetPaletteCategory()
{
	Super::GetPaletteCategory();
	return FText::FromString("Panel");
}

void UMeshSelectionScrollBox::ReleaseSlateResources(bool breleaseresource) {
	Super::ReleaseSlateResources(breleaseresource);
	if (ScrollBar) {
		ScrollBar.Reset();
	}
}

void UMeshSelectionScrollBox::HandleOnMeshSelected(const FMeshData& MeshData)
{
	OnMeshSelectEvent.ExecuteIfBound(MeshData);
}

void UMeshSelectionScrollBox::HandleOnMaterialSelected(const FMaterialData& MaterialData)
{

	OnMaterialSelectEvent.ExecuteIfBound(MaterialData);
}

void UMeshSelectionScrollBox::HandleOnTextureSelected(const FTextureData& TextureData)
{

	OnTextureSelectEvent.ExecuteIfBound(TextureData);
}

