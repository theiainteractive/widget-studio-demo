// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WSModernCard.h"

#include "Blueprint/WidgetTree.h"
#include "Components/OverlaySlot.h"
#include "Components/ScaleBoxSlot.h"
#include "Components/SizeBoxSlot.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Engine.h"

#define LOCTEXT_NAMESPACE "WidgetStudio"

#if WITH_EDITOR
const FText UWidgetStudioModernCard::GetPaletteCategory()
{
	return LOCTEXT("WidgetStudio", "*Widget Studio - Modern");
}
#endif

#undef LOCTEXT_NAMESPACE

TSharedRef<SWidget> UWidgetStudioModernCard::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;
	
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{
		
		// Construct Widgets
		SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Size Box"));
		ScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Scale Box"));
		Overlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Overlay"));
		DropShadow = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("DropShadow"));
		Background = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Background"));
			
		// Construct Hierarchy
		RootWidget->AddChild(SizeBox);
		SizeBox->AddChild(ScaleBox);
		ScaleBox->AddChild(Overlay);
		Overlay->AddChild(DropShadow);
		Overlay->AddChild(Background);

		// Additional Construction Parameters
		bCanOverrideDimensions = true;
		MinimumDimensions = FVector2D(16, 16);

		InitializeStyling();

	}
	return Widget;
}

int32 UWidgetStudioModernCard::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if (!bDisablePainting)
	{
		// Smoothly update Size Box
		UWidgetStudioFunctionLibrary::InterpSizeBoxMinOverrides(SizeBox, GetDimensions().X, GetDimensions().Y, 0);
		
		/* Smoothly lerp the background color based on current states and properties */
		UWidgetStudioFunctionLibrary::InterpImageColor(Background, Color, AnimationTime);
		
		// Smoothly lerp corner style
		const float NewRoundness = UWidgetStudioFunctionLibrary::GetBorderRadius();
		UWidgetStudioFunctionLibrary::InterpBrushImageSize(DropShadow, NewRoundness, NewRoundness, AnimationTime);
		UWidgetStudioFunctionLibrary::InterpBrushImageSize(Background, NewRoundness, NewRoundness, AnimationTime);

		// Smoothly update DropShadow opacity
		const float NewDropShadowOpacity = ShadowStyle != EShadowStyle::None  ? 0.5f  : 0.f;
		UWidgetStudioFunctionLibrary::InterpWidgetOpacity(DropShadow, NewDropShadowOpacity, AnimationTime);
		
		// Smoothly update DropShadow location
		const float NewDropShadowLoc = ShadowStyle == EShadowStyle::Long ? 5.f : 3.0f;
		UWidgetStudioFunctionLibrary::InterpWidgetTranslation(DropShadow, FVector2D(0, IsHovered() && bEnableShadowHoverAnimation ? NewDropShadowLoc + 2.f : NewDropShadowLoc), AnimationTime);
	}

	return LayerId;
}

void UWidgetStudioModernCard::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UpdateStyling();
}

void UWidgetStudioModernCard::InitializeStyling()
{
	Super::InitializeStyling();
	const float RoundedSize = UWidgetStudioFunctionLibrary::GetBorderRadius();
	
	// Setup Size Box
	SizeBox->SetMinDesiredWidth(GetDimensions().X);
	SizeBox->SetMaxDesiredHeight(GetDimensions().Y);
	Cast<UOverlaySlot>(SizeBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(SizeBox->Slot)->SetVerticalAlignment(VAlign_Fill);

	// Setup Scale Box
	ScaleBox->SetStretch(EStretch::Fill);
	Cast<USizeBoxSlot>(ScaleBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<USizeBoxSlot>(ScaleBox->Slot)->SetVerticalAlignment(VAlign_Fill);

	// Setup Overlay
	Cast<UScaleBoxSlot>(Overlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UScaleBoxSlot>(Overlay->Slot)->SetVerticalAlignment(VAlign_Fill);

	// Setup DropShadow
	DropShadow->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedShadowMaterial());
	DropShadow->SetColorAndOpacity(FLinearColor::Black);
	DropShadow->SetRenderOpacity(BaseDropShadowOpacity);
	DropShadow->SetRenderOpacity(0.35f);
	DropShadow->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
	DropShadow->Brush.DrawAs = ESlateBrushDrawType::Box;
	DropShadow->SetBrushSize(FVector2D(RoundedSize, RoundedSize));
	DropShadow->Brush.SetImageSize(FVector2D(RoundedSize, RoundedSize));
	Cast<UOverlaySlot>(DropShadow->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(DropShadow->Slot)->SetVerticalAlignment(VAlign_Fill);
	Cast<UOverlaySlot>(DropShadow->Slot)->SetPadding(FMargin(1, 1, 1, 0));

	// Setup Background
	Background->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
	Background->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(Color));
	Background->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
	Background->Brush.DrawAs = ESlateBrushDrawType::Box;
	Background->Brush.SetImageSize(FVector2D(RoundedSize, RoundedSize));
	Background->SetBrushSize(FVector2D(RoundedSize, RoundedSize));
	Cast<UOverlaySlot>(Background->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(Background->Slot)->SetVerticalAlignment(VAlign_Fill);
}

void UWidgetStudioModernCard::UpdateStyling()
{
	if (SizeBox)
	{
		SizeBox->SetMinDesiredWidth(GetDimensions().X);
		SizeBox->SetMaxDesiredHeight(GetDimensions().Y);
	}
}

EPalette UWidgetStudioModernCard::GetColor() const
{
	return Color;
}

bool UWidgetStudioModernCard::IsShadowHoverAnimationEnabled() const
{
	return bEnableShadowHoverAnimation;
}

void UWidgetStudioModernCard::SetShadowHoverAnimationEnabled(const bool NewState)
{
	bEnableShadowHoverAnimation = NewState;
	SynchronizeProperties();
}

void UWidgetStudioModernCard::SetColor(const EPalette NewColor)
{
	Color = NewColor;
}
