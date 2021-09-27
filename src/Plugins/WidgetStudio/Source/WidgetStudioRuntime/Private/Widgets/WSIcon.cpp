// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WSIcon.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Overlay.h"
#include "Components/ScaleBoxSlot.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Widgets/Images/SImage.h"

#define LOCTEXT_NAMESPACE "WidgetStudio"

#if WITH_EDITOR
const FText UWidgetStudioIcon::GetPaletteCategory()
{
	return LOCTEXT("WidgetStudio", "*Widget Studio");
}
#endif

#undef LOCTEXT_NAMESPACE

TSharedRef<SWidget> UWidgetStudioIcon::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;
	
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{
		
		/* Construct Widgets */
		SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("SizeBox"));
		ScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("ScaleBox"));
		IconItem = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("IconItem"));

			
		/* Construct Hierarchy */
		RootWidget->AddChild(SizeBox);
		SizeBox->AddChild(ScaleBox);
		ScaleBox->AddChild(IconItem);

		// Make the icon not hit testable by default.
		RootWidget->SetVisibility(ESlateVisibility::HitTestInvisible);

		ScaleBox->SetStretch(EStretch::ScaleToFit);
		Cast<USizeBoxSlot>(ScaleBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<USizeBoxSlot>(ScaleBox->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<UScaleBoxSlot>(IconItem->Slot)->SetHorizontalAlignment(HAlign_Center);
		Cast<UScaleBoxSlot>(IconItem->Slot)->SetVerticalAlignment(VAlign_Center);
		
		// Additional Construction Parameters
		bCanOverrideDimensions = false;

		InitializeStyling();
	}
	return Widget;
}

int32 UWidgetStudioIcon::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	if (!bDisablePainting)
	{
		// Adjust SizeBox dimensions
		UWidgetStudioFunctionLibrary::InterpSizeBoxOverrides(SizeBox, Size, Size, AnimationTime);

		// Adjusts icon color
		if (bUsePaletteColor)
		{
			UWidgetStudioFunctionLibrary::InterpImageColor(IconItem, Color, AnimationTime);
		}
	}
	return LayerId;
}

void UWidgetStudioIcon::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UpdateStyling();
}

void UWidgetStudioIcon::InitializeStyling()
{
	Super::InitializeStyling();

	SizeBox->SetWidthOverride(Size);
	SizeBox->SetHeightOverride(Size);

	// Setup Colors
	if (bUsePaletteColor)
	{
		IconItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(Color));
	}
}

void UWidgetStudioIcon::UpdateStyling()
{
	Super::UpdateStyling();

	if (IconItem)
	{
		if (CustomIcon)
		{
			IconItem->SetBrushFromTexture(CustomIcon, true);
		}
		else
		{
			IconItem->SetBrushFromTexture(UWidgetStudioFunctionLibrary::GetTextureFromIconLibrary(Icon), true);
		}

		if (bDisablePainting)
		{
			if (bUsePaletteColor)
			{
				IconItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(Color));
			}
		}
	}

	if (SizeBox)
	{
		if (bDisablePainting)
		{
			SizeBox->SetWidthOverride(Size);
			SizeBox->SetHeightOverride(Size);
		}
	}
}

EIconItem UWidgetStudioIcon::GetIcon() const
{
	return Icon;
}

EPalette UWidgetStudioIcon::GetColor() const
{
	return Color;
}

bool UWidgetStudioIcon::IsUsingCustomIcon() const
{
	return CustomIcon != nullptr;
}

UTexture2D* UWidgetStudioIcon::GetCustomIcon() const
{
	return CustomIcon;
}

float UWidgetStudioIcon::GetSize() const
{
	return Size;
}

float UWidgetStudioIcon::IsUsingPaletteColor() const
{
	return bUsePaletteColor;
}

void UWidgetStudioIcon::SetUsePaletteColor(const bool NewState)
{
	bUsePaletteColor = NewState;
	SynchronizeProperties();
}

void UWidgetStudioIcon::SetSize(const float NewSize)
{
	Size = NewSize;
	SynchronizeProperties();
}

void UWidgetStudioIcon::SetIcon(const EIconItem NewIcon)
{
	Icon = NewIcon;
	SynchronizeProperties();
	
}

void UWidgetStudioIcon::SetColor(const EPalette NewColor)
{
	Color = NewColor;
	SynchronizeProperties();
}

void UWidgetStudioIcon::SetCustomIcon(UTexture2D* NewCustomIcon)
{
	CustomIcon = NewCustomIcon;
	SynchronizeProperties();
}

void UWidgetStudioIcon::ClearCustomIcon()
{
	IconItem->SetBrushFromTexture(UWidgetStudioFunctionLibrary::GetTextureFromIconLibrary(Icon));
	CustomIcon = nullptr;
	SynchronizeProperties();
}

