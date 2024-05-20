// Fill out your copyright notice in the Description page of Project Settings.


#include "SMeshSelectionScrollBox.h"
#include "SlateOptMacros.h"

#define LOCTEXT_NAMESPACE "MyNamespace"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMeshSelectionScrollBox::Construct(const FArguments& InArgs)
{

	MeshAssetManager = InArgs._InMeshAssetManager;
	AssetType = InArgs._InAssetType;
	ImageSize = InArgs._InImageSize;
	HeadTextSize = InArgs._InHeadTextSize;
	HeadString = InArgs._InHeadText;
	ThumbnailTextSize = InArgs._InThumbnailTextSize;
	BGColor = InArgs._InBGColor;

	TSharedPtr<SBorder> RootBorder = SNew(SBorder);

	RootVerticalBox = SNew(SVerticalBox);

	FSlateBrush* BorderImage = new FSlateBrush();
	BorderImage->DrawAs = ESlateBrushDrawType::Type::RoundedBox;
	FSlateBrushOutlineSettings OutlineSettings{};
	OutlineSettings.CornerRadii = FVector4{ 5,5,5,5 };
	OutlineSettings.RoundingType = ESlateBrushRoundingType::Type::FixedRadius;
	BorderImage->OutlineSettings = OutlineSettings;

	RootBorder->SetBorderImage(BorderImage);
	RootBorder->SetContent(RootVerticalBox.ToSharedRef());
	//RootBorder->SetBorderBackgroundColor(FColor::FromHex("#D3D3D3FF"));
	RootBorder->SetBorderBackgroundColor(BGColor);



	ChildSlot.VAlign(EVerticalAlignment::VAlign_Center)
		[
			RootBorder.ToSharedRef()
		];

	HeadText = SNew(STextBlock).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), HeadTextSize))
		.ColorAndOpacity(FColor::FromHex("#0d4cbfFF"));
	HeadText->SetText(FText::FromString(HeadString));

	ScrollBox = SNew(SScrollBox);
	ScrollBox->SetOrientation(EOrientation::Orient_Horizontal);
	CreateScrollBox();
}

void SMeshSelectionScrollBox::CreateScrollBox()
{

	ScrollBox->ClearChildren();

	if (MeshAssetManager.IsValid()) {

		switch (AssetType)
		{
		case EAssetType::Mesh:
		{
			CreateMeshAssetScrollBox();
			break;
		}

		case EAssetType::Material:
		{
			CreateMaterialScrollBox();
			break;
		}

		case EAssetType::Texture:
		{
			CreateTextureScrollBox();
			break;
		}
		}
	}
}

void SMeshSelectionScrollBox::CreateMeshAssetScrollBox()
{

	for (const FMeshData& MeshData : MeshAssetManager->MeshDataArray) {

		if (MeshData.Thumbnail) {

			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);

			TSharedPtr<SBorder> ImageBorder = SNew(SBorder);

			FSlateBrush* BorderImage = new FSlateBrush();
			BorderImage->DrawAs = ESlateBrushDrawType::Type::RoundedBox;
			FSlateBrushOutlineSettings OutlineSettings{};
			OutlineSettings.CornerRadii = FVector4{ 5,5,5,5 };
			OutlineSettings.RoundingType = ESlateBrushRoundingType::Type::FixedRadius;
			BorderImage->OutlineSettings = OutlineSettings;



			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(MeshData.Thumbnail);

			if (UTexture2D* newThumbnail = Cast<UTexture2D>(MeshData.Thumbnail))
			{
				ThumbnailBrush->SetImageSize(FVector2D(ImageSize));
				TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).OnMouseButtonDown_Lambda([this, MeshData](const FGeometry& InGeometry, const FPointerEvent& MouseEvent) {
					if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
						OnMeshSelected.ExecuteIfBound(MeshData);
						return FReply::Handled();
					}
					return FReply::Unhandled();
					}).Cursor(EMouseCursor::Hand);

					ImageBorder->SetContent(ThumbnailImage.ToSharedRef());
					ImageBorder->SetBorderImage(BorderImage);
					ImageBorder->SetBorderBackgroundColor(FColor::Cyan);

					VerticalBox->AddSlot()
						[
							ImageBorder.ToSharedRef()
						];
			}


			TSharedPtr<STextBlock> TextBlock = SNew(STextBlock).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), ThumbnailTextSize))
				.ColorAndOpacity(FSlateColor(FColor::FromHex("#05631eFF")));;
			TextBlock->SetText(FText::FromString(MeshData.Name));
			VerticalBox->AddSlot().HAlign(EHorizontalAlignment::HAlign_Center).AutoHeight()
				[
					TextBlock.ToSharedRef()
				];

			ScrollBox->AddSlot().VAlign(EVerticalAlignment::VAlign_Center).Padding(FVector2D(5))
				[
					VerticalBox.ToSharedRef()
				];

		}
	}

	RootVerticalBox->AddSlot().AutoHeight().HAlign(EHorizontalAlignment::HAlign_Center)
		[
			HeadText.ToSharedRef()
		];

	RootVerticalBox->AddSlot().AutoHeight()
		[
			ScrollBox.ToSharedRef()
		];
}

void SMeshSelectionScrollBox::CreateMaterialScrollBox()
{

	for (const FMaterialData& MaterialData : MeshAssetManager->MaterialDataArray) {

		if (MaterialData.Thumbnail) {

			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);

			TSharedPtr<SBorder> ImageBorder = SNew(SBorder);

			FSlateBrush* BorderImage = new FSlateBrush();
			BorderImage->DrawAs = ESlateBrushDrawType::Type::RoundedBox;
			FSlateBrushOutlineSettings OutlineSettings{};
			OutlineSettings.CornerRadii = FVector4{ 5,5,5,5 };
			OutlineSettings.RoundingType = ESlateBrushRoundingType::Type::FixedRadius;
			BorderImage->OutlineSettings = OutlineSettings;

			FSlateBrush* ThumbnailBrush = new FSlateBrush();

			ThumbnailBrush->SetResourceObject(MaterialData.Thumbnail);

			if (UTexture2D* newThumbnail = Cast<UTexture2D>(MaterialData.Thumbnail))
			{
				ThumbnailBrush->SetImageSize(FVector2D(ImageSize));
				TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).OnMouseButtonDown_Lambda([this, MaterialData](const FGeometry& InGeometry, const FPointerEvent& MouseEvent) {
					if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
						OnMaterialSelected.ExecuteIfBound(MaterialData);
						return FReply::Handled();
					}
					return FReply::Unhandled();
					}).Cursor(EMouseCursor::Hand);

					ImageBorder->SetContent(ThumbnailImage.ToSharedRef());
					ImageBorder->SetBorderImage(BorderImage);
					//ImageBorder->SetBorderBackgroundColor(FColor::Cyan);

					VerticalBox->AddSlot()
						[
							ImageBorder.ToSharedRef()
						];
			}

			TSharedPtr<STextBlock> TextBlock = SNew(STextBlock).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), ThumbnailTextSize))
				.ColorAndOpacity(FSlateColor(FColor::FromHex("#05631eFF")));;
			TextBlock->SetText(FText::FromString(MaterialData.Name));
			VerticalBox->AddSlot().HAlign(EHorizontalAlignment::HAlign_Center).AutoHeight()
				[
					TextBlock.ToSharedRef()
				];

			ScrollBox->AddSlot().VAlign(EVerticalAlignment::VAlign_Center).Padding(FVector2D(5))
				[
					VerticalBox.ToSharedRef()
				];
		}
	}
	RootVerticalBox->AddSlot().AutoHeight().HAlign(EHorizontalAlignment::HAlign_Center)
		[
			HeadText.ToSharedRef()
		];

	RootVerticalBox->AddSlot().AutoHeight()
		[
			ScrollBox.ToSharedRef()
		];
}

void SMeshSelectionScrollBox::CreateTextureScrollBox()
{

	for (const FTextureData& TextureData : MeshAssetManager->TextureDatalArray) {

		if (TextureData.Texture) {

			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);


			TSharedPtr<SBorder> ImageBorder = SNew(SBorder);

			FSlateBrush* BorderImage = new FSlateBrush();
			BorderImage->DrawAs = ESlateBrushDrawType::Type::RoundedBox;
			FSlateBrushOutlineSettings OutlineSettings{};
			OutlineSettings.CornerRadii = FVector4{ 5,5,5,5 };
			OutlineSettings.RoundingType = ESlateBrushRoundingType::Type::FixedRadius;
			BorderImage->OutlineSettings = OutlineSettings;


			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(TextureData.Texture);

			if (UTexture2D* newThumbnail = Cast<UTexture2D>(TextureData.Texture))
			{
				ThumbnailBrush->SetImageSize(FVector2D(ImageSize));
				TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).OnMouseButtonDown_Lambda([this, TextureData](const FGeometry& InGeometry, const FPointerEvent& MouseEvent) {
					if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
						OnTextureSelected.ExecuteIfBound(TextureData);
						return FReply::Handled();
					}
					return FReply::Unhandled();
					}).Cursor(EMouseCursor::Hand);

					ImageBorder->SetContent(ThumbnailImage.ToSharedRef());
					ImageBorder->SetBorderImage(BorderImage);
					//ImageBorder->SetBorderBackgroundColor(FColor::Cyan);

					VerticalBox->AddSlot()
						[
							ImageBorder.ToSharedRef()
						];
			}

			TSharedPtr<STextBlock> TextBlock = SNew(STextBlock).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), ThumbnailTextSize))
				.ColorAndOpacity(FSlateColor(FColor::FromHex("#05631eFF")));;
			TextBlock->SetText(FText::FromString(TextureData.Name));
			VerticalBox->AddSlot().HAlign(EHorizontalAlignment::HAlign_Center).AutoHeight()
				[
					TextBlock.ToSharedRef()
				];

			ScrollBox->AddSlot().VAlign(EVerticalAlignment::VAlign_Center).Padding(FVector2D(5))
				[
					VerticalBox.ToSharedRef()
				];
		}
	}
	RootVerticalBox->AddSlot().AutoHeight().HAlign(EHorizontalAlignment::HAlign_Center)
		[
			HeadText.ToSharedRef()
		];

	RootVerticalBox->AddSlot().AutoHeight()
		[
			ScrollBox.ToSharedRef()
		];
}

#undef LOCTEXT_NAMESPACE
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
