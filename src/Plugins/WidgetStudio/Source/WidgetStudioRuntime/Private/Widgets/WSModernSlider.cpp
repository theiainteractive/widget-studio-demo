// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WSModernSlider.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/OverlaySlot.h"
#include "Components/ScaleBoxSlot.h"
#include "Components/SizeBoxSlot.h"
#include "Components/VerticalBoxSlot.h"
#include "Engine/Font.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Engine/Engine.h"

#define LOCTEXT_NAMESPACE "WidgetStudio"

#if WITH_EDITOR
const FText UWidgetStudioModernSlider::GetPaletteCategory()
{
	return LOCTEXT("WidgetStudio", "*Widget Studio - Modern");
}
#endif

#undef LOCTEXT_NAMESPACE

TSharedRef<SWidget> UWidgetStudioModernSlider::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;
	
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{

		/*
		* RootWidget (InvalidationBox)
		* - Size Box
		* -- HBox
		* --- Overlay
		* ---- Track Scale Box
		* ----- Track Overlay
		* ------ Track Drop Shadow
		* ------ Track
		* ---- Fill Track Size Box
		* ----- Fill Track
		* ---- Handle Scale Box
		* ----- Handle Size Box
		* ------ Handle Overlay
		* ------- Handle Drop Shadow
		* ------- Handle
		* ---- SliderItem
		* --- Text Input Size Box
		* ---- Text Input
		*/
		
		/* Construct Widgets */
		LabelItem = WidgetTree->ConstructWidget<UWidgetStudioText>(UWidgetStudioText::StaticClass(), TEXT("Label Item"));
		SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Size Box"));
		HBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("HBox"));
		Overlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Overlay"));
		TrackScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Track Scale Box"));
		TrackOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Track Overlay"));
		TrackDropShadow = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Track DropShadow"));
		Track = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Track0"));
		FillTrackSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Fill Track Size Box"));
		FillTrack = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Fill Track"));
		HandleScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Handle Scale Box"));
		HandleSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Handle Size Box"));
		HandleOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Handle Overlay"));
		HandleDropShadow = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Handle DropShadow"));
		Handle = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Handle"));
		SliderItem = WidgetTree->ConstructWidget<USlider>(USlider::StaticClass(), TEXT("Slider Item"));
		TextInputSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Text Input Size Box"));
		TextInput = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("Text Input"));

		/* Construct Hierarchy */
		RootWidget->AddChild(LabelItem);
		RootWidget->AddChild(SizeBox);
		SizeBox->AddChild(HBox);
		HBox->AddChild(Overlay);
		
		Overlay->AddChild(TrackScaleBox);
		TrackScaleBox->AddChild(TrackOverlay);
		TrackOverlay->AddChild(TrackDropShadow);
		TrackOverlay->AddChild(Track);

		Overlay->AddChild(FillTrackSizeBox);
		FillTrackSizeBox->AddChild(FillTrack);

		Overlay->AddChild(HandleScaleBox);
		HandleScaleBox->AddChild(HandleSizeBox);
		HandleSizeBox->AddChild(HandleOverlay);
		HandleOverlay->AddChild(HandleDropShadow);
		HandleOverlay->AddChild(Handle);

		Overlay->AddChild(SliderItem);

		HBox->AddChild(TextInputSizeBox);
		TextInputSizeBox->AddChild(TextInput);

		// Additional Construction Parameters
		MinimumDimensions = FVector2D(225, 13);

		InitializeStyling();

	}
	return Widget;
}

int32 UWidgetStudioModernSlider::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	if (!bDisablePainting)
	{
		// Smoothly lerp size box
		UWidgetStudioFunctionLibrary::InterpSizeBoxMinOverrides(SizeBox, GetDimensions().X, 0, AnimationTime);
		
		// Smoothly lerp Track Color
		UWidgetStudioFunctionLibrary::InterpImageColor(Track, TrackColor, AnimationTime);

		// Smoothly lerp Fill Track Color
		const EPalette NewFillColor = bColorBasedOnValue ? UWidgetStudioFunctionLibrary::GetPaletteBasedValue(Value / Max, bReverseColorOrder) : HandleColor;
		UWidgetStudioFunctionLibrary::InterpImageColor(FillTrack, NewFillColor, AnimationTime);

		// Smoothly lerp Handle Color
		UWidgetStudioFunctionLibrary::InterpImageColor(Handle, NewFillColor, AnimationTime);
		
		/* Smoothly lerp Text Color to match track color */
		UWidgetStudioFunctionLibrary::InterpEditableTextBoxColor(TextInput, TextColor, AnimationTime);
		UWidgetStudioFunctionLibrary::InterpWidgetColor(LabelItem, TextColor, AnimationTime);

		/* Smoothly lerp handle based on hover state */
		UWidgetStudioFunctionLibrary::InterpSizeBoxOverrides(HandleSizeBox, IsHovered() ? HandleSizeBox->HeightOverride * 2.5 : HandleSizeBox->HeightOverride, 0, AnimationTime * 3);

		/* Smoothly lerp handle location based on value/max */
		const float HandleHalfWidth = IsHandleVisible() ? HandleOverlay->GetPaintSpaceGeometry().GetLocalSize().X / 2 : 0;
		const float NewHandleLoc = UKismetMathLibrary::MapRangeClamped(
			SliderItem->GetValue(),
			SliderItem->MinValue,
			SliderItem->MaxValue,
			0,
			Track->GetPaintSpaceGeometry().GetLocalSize().X - (bDisplayHandle ? HandleSizeBox->WidthOverride : 0)
			);

		UWidgetStudioFunctionLibrary::InterpWidgetTranslation(HandleOverlay, FVector2D(NewHandleLoc, 0), AnimationTime * 4);
		
		/* Smoothly lerp fill track location based on value/max */
		UWidgetStudioFunctionLibrary::InterpSizeBoxOverrides(FillTrackSizeBox, NewHandleLoc + HandleHalfWidth, 0, AnimationTime * 4, false);

		/* Smoothly lerp handle drop shadow opacity based on hover state */
		UWidgetStudioFunctionLibrary::InterpWidgetOpacity(HandleDropShadow, IsHovered() ? .35 : .1, AnimationTime);

		/* Smoothly lerp handle drop shadow opacity based on hover state */
		UWidgetStudioFunctionLibrary::InterpWidgetTranslation(HandleDropShadow, FVector2D(0, IsHovered() ? 5.f : 1.f), AnimationTime);
	}
	return LayerId;
}

void UWidgetStudioModernSlider::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UpdateStyling();
}

void UWidgetStudioModernSlider::InitializeStyling()
{
	Super::InitializeStyling();

	/* Apply Styling */
		const float Height = GetDimensions().Y * .25f;
		const float Size = GetDimensions().Y * .5f;

		/* Setup Size Box */
		//SizeBox->SetMinDesiredHeight(Height);
		SizeBox->SetMinDesiredWidth(GetDimensions().X);
		Cast<UVerticalBoxSlot>(SizeBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UVerticalBoxSlot>(SizeBox->Slot)->SetVerticalAlignment(VAlign_Fill);

		// Setup Label
		LabelItem->bDisablePainting = true;
		LabelItem->SetColor(EPalette::White);
		LabelItem->SetType(EFontType::Caption);
		Cast<UVerticalBoxSlot>(LabelItem->Slot)->SetPadding(FMargin(0, 0, 0, 3));

		/* Setup HBox */
		Cast<USizeBoxSlot>(HBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<USizeBoxSlot>(HBox->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<USizeBoxSlot>(HBox->Slot)->SetPadding(FVector4(0, 5, 0, 5));

		/* Setup Overlay */
		Cast<UHorizontalBoxSlot>(Overlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UHorizontalBoxSlot>(Overlay->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<UHorizontalBoxSlot>(Overlay->Slot)->SetSize(ESlateSizeRule::Fill);

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
		TrackDropShadow->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		TrackDropShadow->Brush.DrawAs = ESlateBrushDrawType::Box;
		TrackDropShadow->Brush.SetImageSize(FVector2D(Height, Height));
		TrackDropShadow->SetRenderTranslation(FVector2D(0.f, 3.f));
		TrackDropShadow->SetColorAndOpacity(FLinearColor::Black);
		TrackDropShadow->SetRenderOpacity(0.3f);
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

		/* Setup Handle Scale Box */
		HandleScaleBox->SetStretch(EStretch::ScaleToFit);
		Cast<UOverlaySlot>(HandleScaleBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(HandleScaleBox->Slot)->SetVerticalAlignment(VAlign_Center);

		/* Setup Handle Size Box */
		HandleSizeBox->SetHeightOverride(Size);
		Cast<UScaleBoxSlot>(HandleSizeBox->Slot)->SetHorizontalAlignment(HAlign_Left);
		Cast<UScaleBoxSlot>(HandleSizeBox->Slot)->SetVerticalAlignment(VAlign_Fill);

		/* Setup Handle Overlay */
		Cast<USizeBoxSlot>(HandleOverlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<USizeBoxSlot>(HandleOverlay->Slot)->SetVerticalAlignment(VAlign_Fill);

		/* Setup Handle Drop Shadow */
		HandleDropShadow->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedShadowMaterial());
		HandleDropShadow->SetBrushSize(FVector2D(Size, Size));
		HandleDropShadow->SetRenderTranslation(FVector2D(0.f, 3.f));
		HandleDropShadow->SetColorAndOpacity(FLinearColor::Black);
		HandleDropShadow->SetRenderOpacity(0.3f);
		HandleDropShadow->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		HandleDropShadow->Brush.DrawAs = ESlateBrushDrawType::Box;
		HandleDropShadow->Brush.SetImageSize(FVector2D(Size, Size));
		Cast<UOverlaySlot>(HandleDropShadow->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(HandleDropShadow->Slot)->SetVerticalAlignment(VAlign_Fill);

		/* Setup Handle */
		Handle->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
		Handle->SetBrushSize(FVector2D(Size, Size));
		Handle->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		Handle->Brush.DrawAs = ESlateBrushDrawType::Box;
		Handle->Brush.SetImageSize(FVector2D(Size, Size));
		Cast<UOverlaySlot>(Handle->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(Handle->Slot)->SetVerticalAlignment(VAlign_Fill);

		/* Setup Slider Item */
		SliderItem->SetRenderOpacity(0); // Hide the Slider as it's only used for interactivity and not display
		SliderItem->WidgetStyle.NormalBarImage.DrawAs = ESlateBrushDrawType::NoDrawType;
		SliderItem->WidgetStyle.HoveredBarImage.DrawAs = ESlateBrushDrawType::NoDrawType;
		SliderItem->WidgetStyle.NormalThumbImage.DrawAs = ESlateBrushDrawType::NoDrawType;
		SliderItem->WidgetStyle.HoveredThumbImage.DrawAs = ESlateBrushDrawType::NoDrawType;
		SliderItem->WidgetStyle.DisabledBarImage.DrawAs = ESlateBrushDrawType::NoDrawType;
		SliderItem->WidgetStyle.DisabledThumbImage.DrawAs = ESlateBrushDrawType::NoDrawType;
		Cast<UOverlaySlot>(SliderItem->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(SliderItem->Slot)->SetVerticalAlignment(VAlign_Fill);

		/* Setup Text Input Size Box */
		TextInputSizeBox->SetWidthOverride(55);
		Cast<UHorizontalBoxSlot>(TextInputSizeBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UHorizontalBoxSlot>(TextInputSizeBox->Slot)->SetVerticalAlignment(VAlign_Fill);

		/* Setup Text Input Box */
		UWidgetStudioTypography* Typography = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>()->GetTypography();
		if (Typography)
		{
			const FFontStyle FontStyle = UWidgetStudioFunctionLibrary::GetTypeScaleFromTypography(EFontType::Caption);

			FSlateFontInfo FontInfo;
			FontInfo.FontObject = Typography->Typeface;
			FontInfo.Size = FontStyle.Size;
			FontInfo.TypefaceFontName = UWidgetStudioFunctionLibrary::GetFontWeightName(FontStyle.Weight);
			FontInfo.LetterSpacing = FontStyle.LetterSpacing;

			TextInput->WidgetStyle.SetFont(FontInfo);
		}
		
		TextInput->SetJustification(ETextJustify::Right);
		TextInput->WidgetStyle.BackgroundImageNormal.DrawAs = ESlateBrushDrawType::NoDrawType;
		TextInput->WidgetStyle.BackgroundImageHovered.DrawAs = ESlateBrushDrawType::NoDrawType;
		TextInput->WidgetStyle.BackgroundImageFocused.DrawAs = ESlateBrushDrawType::NoDrawType;
		TextInput->WidgetStyle.BackgroundImageReadOnly.DrawAs = ESlateBrushDrawType::NoDrawType;

		// Setup Colors
		const EPalette NewFillColor = bColorBasedOnValue ? UWidgetStudioFunctionLibrary::GetPaletteBasedValue(Value / Max, bReverseColorOrder) : HandleColor;
		Track->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(TrackColor));
		FillTrack->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(NewFillColor));
		Handle->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(NewFillColor));
		TextInput->WidgetStyle.SetForegroundColor(UWidgetStudioFunctionLibrary::GetColorFromPalette(TextColor));
		LabelItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(TextColor));

		/* Setup Bindings */
		SliderItem->OnValueChanged.RemoveDynamic(this, &UWidgetStudioModernSlider::SyncSliderValue);
		TextInput->OnTextCommitted.RemoveDynamic(this, &UWidgetStudioModernSlider::OnTextCommitted);

		SliderItem->OnValueChanged.AddDynamic(this, &UWidgetStudioModernSlider::SyncSliderValue);
		TextInput->OnTextCommitted.AddDynamic(this, &UWidgetStudioModernSlider::OnTextCommitted);
}

void UWidgetStudioModernSlider::UpdateStyling()
{
	Super::UpdateStyling();

	if (SliderItem)
	{
		SliderItem->SetMinValue(Min);
		SliderItem->SetMaxValue(Max);
		SliderItem->SetStepSize(StepSize);
		SliderItem->SetIsEnabled(bEnableInput);
		SliderItem->SetValue(FilterValue(Value));
	}

	if (TextInputSizeBox)
	{
		TextInputSizeBox->SetVisibility(bDisplayValue ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (TextInput)
	{
		TextInput->SetIsEnabled(bEnableInput);
		TextInput->SetText(UKismetTextLibrary::Conv_FloatToText(
			FilterValue(Value),
			HalfToEven,
			false,
			true,
			1,
			324,
			0,
			2
		));
	}

	if (HandleScaleBox)
	{
		HandleScaleBox->SetVisibility(bDisplayHandle ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (LabelItem)
	{
		LabelItem->SetText(Label);
		LabelItem->SetVisibility(Label.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

		LabelItem->SizeModifier = SizeModifier;
	}
}

float UWidgetStudioModernSlider::FilterValue(const float InValue) const
{
	float TempValue = InValue;

	// Snap to Step Size
	TempValue = UKismetMathLibrary::GridSnap_Float(TempValue, StepSize);

	// Process Enumeration Type
	if (Enumeration == EEnumeration::Int)
	{
		TempValue = UKismetMathLibrary::Clamp(TempValue, Min, Max);
	}
	else
	{
		TempValue = UKismetMathLibrary::FClamp(TempValue, Min, Max);
	}

	return TempValue;
}

void UWidgetStudioModernSlider::OnTextCommitted(const FText& InText, const ETextCommit::Type InCommitMethod)
{
	if (InText.IsNumeric())
	{
		if (InCommitMethod == ETextCommit::OnEnter || InCommitMethod == ETextCommit::OnUserMovedFocus)
		{
			SetValue(UKismetStringLibrary::Conv_StringToFloat(UKismetTextLibrary::Conv_TextToString(InText)));
		}
	}
	else
	{
		SetValue(Value);
	}
}

void UWidgetStudioModernSlider::SyncSliderValue(const float NewValue)
{
	SetValue(NewValue, true);
}

FText UWidgetStudioModernSlider::GetLabelText() const
{
	return Label;
}

void UWidgetStudioModernSlider::SetLabelText(const FText NewText)
{
	Label = NewText;
	SynchronizeProperties();
}

float UWidgetStudioModernSlider::GetValue() const
{
	return Value;
}

float UWidgetStudioModernSlider::GetMinValue() const
{
	return Min;
}

float UWidgetStudioModernSlider::GetMaxValue() const
{
	return Max;
}

float UWidgetStudioModernSlider::GetStepSize() const
{
	return StepSize;
}

bool UWidgetStudioModernSlider::IsValueVisible() const
{
	return bDisplayValue;
}

bool UWidgetStudioModernSlider::IsHandleVisible() const
{
	return bDisplayHandle;
}

bool UWidgetStudioModernSlider::IsColorBasedOnValue() const
{
	return bColorBasedOnValue;
}

bool UWidgetStudioModernSlider::IsColorBasedOnValueReversed() const
{
	return bReverseColorOrder;
}

void UWidgetStudioModernSlider::SetReverseColorOrder(const bool bNewState)
{
	bReverseColorOrder = bNewState;
}

EEnumeration UWidgetStudioModernSlider::GetEnumerationType() const
{
	return Enumeration;
}

bool UWidgetStudioModernSlider::IsInputEnabled() const
{
	return bEnableInput;
}

bool UWidgetStudioModernSlider::IsIncrementLocked() const
{
	return bLockIncrement;
}

bool UWidgetStudioModernSlider::IsDecrementLocked() const
{
	return bLockDecrement;
}

void UWidgetStudioModernSlider::SetLockIncrement(const bool NewState)
{
	bLockIncrement = NewState;
}

void UWidgetStudioModernSlider::SetLockDecrement(const bool NewState)
{
	bLockDecrement = NewState;
}

void UWidgetStudioModernSlider::SetInputState(const bool NewState)
{
	bEnableInput = NewState;
	SynchronizeProperties();
	
}

void UWidgetStudioModernSlider::SetEnumerationType(const EEnumeration NewType)
{
	Enumeration = NewType;
	SynchronizeProperties();
}

void UWidgetStudioModernSlider::SetValue(const float NewValue, const bool bTriggerValueChangedEvent)
{
	// If incrementing is locked, set the slider to the stored value.
	if (bLockIncrement && NewValue > Value)
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("%s: Attempted to increase the value when incrementing is locked."), *FString(__func__))
		SliderItem->SetValue(Value);
		SynchronizeProperties();
		return;
	}
	
	// If decrementing is locked, set the slider to the stored value.
	if (bLockDecrement && NewValue < Value)
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("%s: Attempted to decrease the value when decrementing is locked."), *FString(__func__))
		SliderItem->SetValue(Value);
		SynchronizeProperties();
		return;
	}
	
	Value = FilterValue(NewValue);

	if (bTriggerValueChangedEvent)
	{
		OnValueChanged.Broadcast(Value);
	}
	
	SynchronizeProperties();
}

void UWidgetStudioModernSlider::SetMinValue(const float NewValue)
{
	Min = NewValue;
	SynchronizeProperties();
}

void UWidgetStudioModernSlider::SetMaxValue(const float NewValue)
{
	Max = NewValue;
	SynchronizeProperties();
}

void UWidgetStudioModernSlider::SetStepSize(const float NewValue)
{
	StepSize = NewValue;
	SynchronizeProperties();
}

void UWidgetStudioModernSlider::SetValueVisibility(const bool bNewState)
{
	bDisplayValue = bNewState;
	SynchronizeProperties();
}

void UWidgetStudioModernSlider::SetHandleVisibility(const bool bNewState)
{
	bDisplayHandle = bNewState;
	SynchronizeProperties();
}

void UWidgetStudioModernSlider::SetColorBasedOnValue(const bool bNewState)
{
	bColorBasedOnValue = bNewState;
	SynchronizeProperties();
}
