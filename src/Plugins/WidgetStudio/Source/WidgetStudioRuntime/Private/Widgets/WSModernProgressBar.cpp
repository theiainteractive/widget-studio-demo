// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WSModernProgressBar.h"

#include "Blueprint/WidgetTree.h"
#include "Components/OverlaySlot.h"
#include "Components/ScaleBoxSlot.h"
#include "Components/SizeBoxSlot.h"
#include "Kismet/KismetMathLibrary.h"

#define LOCTEXT_NAMESPACE "WidgetStudio"

#if WITH_EDITOR
const FText UWidgetStudioModernProgressBar::GetPaletteCategory()
{
	return LOCTEXT("WidgetStudio", "*Widget Studio - Modern");
}
#endif

#undef LOCTEXT_NAMESPACE

TSharedRef<SWidget> UWidgetStudioModernProgressBar::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;
	
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{

		/*
		* RootWidget (InvalidationBox)
		* - Size Box
		* -- Overlay
		* --- Track Scale Box
		* ---- Track Overlay
		* ----- Track Drop Shadow
		* ----- Track
		* --- Fill Track Size Box
		* ---- Fill Track
		* --- Indicator Size Box
		* ---- Indicator
		*/
		
		/* Construct Widgets */
		SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Size Box"));
		Overlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Overlay"));
		TrackScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Track Scale Box"));
		TrackOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Track Overlay"));
		TrackDropShadow = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Track DropShadow"));
		Track = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Track0"));
		FillTrackSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Fill Track Size Box"));
		FillTrack = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Fill Track"));
		IndicatorSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Indicator Size Box"));
		Indicator = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Indicator"));

		/* Construct Hierarchy */
		RootWidget->AddChild(SizeBox);
		SizeBox->AddChild(Overlay);
		
		Overlay->AddChild(TrackScaleBox);
		TrackScaleBox->AddChild(TrackOverlay);
		TrackOverlay->AddChild(TrackDropShadow);
		TrackOverlay->AddChild(Track);

		Overlay->AddChild(FillTrackSizeBox);
		FillTrackSizeBox->AddChild(FillTrack);

		Overlay->AddChild(IndicatorSizeBox);
		IndicatorSizeBox->AddChild(Indicator);

		// Additional Construction Parameters
		MinimumDimensions = FVector2D(225, 13);

		InitializeStyling();
	}
	return Widget;
}

int32 UWidgetStudioModernProgressBar::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if (!bDisablePainting)
	{
		/* Smoothly update size box */
		UWidgetStudioFunctionLibrary::InterpSizeBoxMinOverrides(SizeBox, GetDimensions().X, 0, AnimationTime);
		
		/* Smoothly lerp Track Color */
		UWidgetStudioFunctionLibrary::InterpImageColor(Track, TrackColor, AnimationTime);

		/* Smoothly lerp Fill Track Color */
		UWidgetStudioFunctionLibrary::InterpImageColor(FillTrack, FillColor, AnimationTime);
		UWidgetStudioFunctionLibrary::InterpWidgetOpacity(FillTrack, Percent > 0.f ? 1.f : 0.f, AnimationTime * 4);

		// Get fill width
		const float NewFillWidth = UKismetMathLibrary::MapRangeClamped(Percent, .0f, 100.f, 0, Track->GetPaintSpaceGeometry().GetLocalSize().X);
		
		/* Smoothly lerp fill track width */
		UWidgetStudioFunctionLibrary::InterpSizeBoxOverrides(FillTrackSizeBox, NewFillWidth, FillTrackSizeBox->HeightOverride, AnimationTime * 2, false);

		/* Smoothly lerp indicator width */
		if (bDisplayIndicator)
		{
			const float TargetIndicatorWidth = bForwardProgress ? NewFillWidth : 0.f;
			float NewIndicatorWidth;

			if (IndicatorSizeBox->WidthOverride == TargetIndicatorWidth)
			{
				NewIndicatorWidth = bForwardProgress ? 0.f : NewFillWidth;
			}
			else
			{
				NewIndicatorWidth = UKismetMathLibrary::FInterpTo(
					IndicatorSizeBox->WidthOverride,
					TargetIndicatorWidth,
					GetWorld()->GetDeltaSeconds(),
					AnimationTime * .5f
				);
			}
			IndicatorSizeBox->SetWidthOverride(NewIndicatorWidth);
			IndicatorSizeBox->SetRenderOpacity(UKismetMathLibrary::MapRangeClamped(NewIndicatorWidth, 0.f, NewFillWidth, bForwardProgress ? 0.5f : 0.0f, bForwardProgress ? 0.0f : 0.75f));
		}
	}
	return LayerId;
}

void UWidgetStudioModernProgressBar::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UpdateStyling();
}

void UWidgetStudioModernProgressBar::InitializeStyling()
{
	Super::InitializeStyling();

	const float Height = GetDimensions().Y * .25f;
		const float Size = GetDimensions().Y * .5f;

		/* Setup Size Box */
		SizeBox->SetMinDesiredWidth(GetDimensions().X);
		Cast<UOverlaySlot>(SizeBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(SizeBox->Slot)->SetVerticalAlignment(VAlign_Fill);

		/* Setup Overlay */
		Cast<USizeBoxSlot>(Overlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<USizeBoxSlot>(Overlay->Slot)->SetVerticalAlignment(VAlign_Fill);

		/* Setup Track Scale Box */
		TrackScaleBox->SetStretch(EStretch::ScaleToFit);
		Cast<UOverlaySlot>(TrackScaleBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(TrackScaleBox->Slot)->SetVerticalAlignment(VAlign_Center);

		/* Setup Track Overlay */
		Cast<UScaleBoxSlot>(TrackOverlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UScaleBoxSlot>(TrackOverlay->Slot)->SetVerticalAlignment(VAlign_Fill);

		/* Setup Track DropShadow */
		TrackDropShadow->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedShadowMaterial());
		TrackDropShadow->SetBrushSize(FVector2D(Height, Height));
		TrackDropShadow->SetRenderTranslation(FVector2D(0.f, 3.f));
		TrackDropShadow->SetColorAndOpacity(FLinearColor::Black);
		TrackDropShadow->SetRenderOpacity(0.3f);
		TrackDropShadow->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		TrackDropShadow->Brush.DrawAs = ESlateBrushDrawType::Box;
		TrackDropShadow->Brush.SetImageSize(FVector2D(Height, Height));
		Cast<UOverlaySlot>(TrackDropShadow->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(TrackDropShadow->Slot)->SetVerticalAlignment(VAlign_Fill);

		/* Setup Track */
		Track->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
		Track->SetBrushSize(FVector2D(Height, Height));
		Track->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		Track->Brush.DrawAs = ESlateBrushDrawType::Box;
		Track->Brush.SetImageSize(FVector2D(Height, Height));
		Cast<UOverlaySlot>(Track->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(Track->Slot)->SetVerticalAlignment(VAlign_Fill);

		/* Setup Fill Track Size Box */
		Cast<UOverlaySlot>(FillTrackSizeBox->Slot)->SetHorizontalAlignment(HAlign_Left);
		Cast<UOverlaySlot>(FillTrackSizeBox->Slot)->SetVerticalAlignment(VAlign_Center);

		/* Setup Fill Track */
		FillTrack->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
		FillTrack->SetBrushSize(FVector2D(Height, Height));
		FillTrack->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		FillTrack->Brush.DrawAs = ESlateBrushDrawType::Box;
		FillTrack->Brush.SetImageSize(FVector2D(Height, Height));
		Cast<USizeBoxSlot>(FillTrack->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<USizeBoxSlot>(FillTrack->Slot)->SetVerticalAlignment(VAlign_Fill);

		/* Setup Indicator Size Box */
		Cast<UOverlaySlot>(IndicatorSizeBox->Slot)->SetHorizontalAlignment(HAlign_Left);
		Cast<UOverlaySlot>(IndicatorSizeBox->Slot)->SetVerticalAlignment(VAlign_Center);

		/* Setup Indicator */
		Indicator->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
		Indicator->SetBrushSize(FVector2D(Height, Height));
		Indicator->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		Indicator->Brush.DrawAs = ESlateBrushDrawType::Box;
		Indicator->Brush.SetImageSize(FVector2D(Height, Height));
		Indicator->SetColorAndOpacity(FLinearColor::White);
		Cast<USizeBoxSlot>(Indicator->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<USizeBoxSlot>(Indicator->Slot)->SetVerticalAlignment(VAlign_Fill);

		IndicatorSizeBox->SetVisibility(bDisplayIndicator ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

		// Setup Colors
		Track->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(TrackColor));
		FillTrack->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(FillColor));
}

void UWidgetStudioModernProgressBar::UpdateStyling()
{
	Super::UpdateStyling();

	if (IndicatorSizeBox)
	{
		IndicatorSizeBox->SetVisibility(bDisplayIndicator ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}

float UWidgetStudioModernProgressBar::GetProgress() const
{
	return Percent;
}

bool UWidgetStudioModernProgressBar::IsIndicatorVisible() const
{
	return bDisplayIndicator;
}

EPalette UWidgetStudioModernProgressBar::GetTrackColor() const
{
	return TrackColor;
}

EPalette UWidgetStudioModernProgressBar::GetFillColor() const
{
	return FillColor;
}

void UWidgetStudioModernProgressBar::SetProgress(const float NewValue)
{
	const float TempValue = UKismetMathLibrary::FClamp(NewValue, 0.0f, 100.0f);
	bForwardProgress = UKismetMathLibrary::GreaterEqual_FloatFloat(TempValue, Percent);
	Percent = TempValue;
	OnProgressChanged.Broadcast(Percent);
}

void UWidgetStudioModernProgressBar::SetIndicatorVisibility(const bool bNewState)
{
	bDisplayIndicator = bNewState;
	SynchronizeProperties();
}

void UWidgetStudioModernProgressBar::SetTrackColor(const EPalette NewColor)
{
	TrackColor = NewColor;
}

void UWidgetStudioModernProgressBar::SetFillColor(const EPalette NewColor)
{
	FillColor = NewColor;
}
