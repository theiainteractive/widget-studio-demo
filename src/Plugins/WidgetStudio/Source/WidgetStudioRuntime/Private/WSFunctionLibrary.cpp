// Fill out your copyright notice in the Description page of Project Settings.


#include "WSFunctionLibrary.h"
#include "WSSubsystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Font.h"
#include "Engine/Engine.h"

/**
* Returns the color from the Palette based on selection.
* @return FLinearColor
*/
FLinearColor UWidgetStudioFunctionLibrary::GetColorFromPalette(const EPalette Color)
{

	UWidgetStudioSubsystem* WSSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	
	// Return blank white if theme is invalid
	if(!WSSubsystem || !WSSubsystem->GetTheme())
	{
		return FLinearColor(1, 1, 1, 1);
	}

	// Accent Colors
	if(Color == EPalette::PrimaryAccent) { return WSSubsystem->GetTheme()->PrimaryAccent; }
	if(Color == EPalette::SecondaryAccent) { return WSSubsystem->GetTheme()->SecondaryAccent; }

	// Content / Foreground Colors
	if(Color == EPalette::PrimaryContent) { return WSSubsystem->GetTheme()->PrimaryContent; }
	if(Color == EPalette::SecondaryContent) { return WSSubsystem->GetTheme()->SecondaryContent; }
	if(Color == EPalette::TertiaryContent) { return WSSubsystem->GetTheme()->TertiaryContent; }

	// Background Colors
	if(Color == EPalette::PrimaryBackground) { return WSSubsystem->GetTheme()->PrimaryBackground; }
	if(Color == EPalette::SecondaryBackground) { return WSSubsystem->GetTheme()->SecondaryBackground; }
	if(Color == EPalette::TertiaryBackground) { return WSSubsystem->GetTheme()->TertiaryBackground; }

	// Basic Colors
	if(Color == EPalette::Red)		{ return FLinearColor(.66, 0, .07, 1); }
	if(Color == EPalette::Orange)	{ return FLinearColor(.99, .32, .18, 1); }
	if(Color == EPalette::Yellow)	{ return FLinearColor(1, .68, 0, 1); }
	if(Color == EPalette::Olive)	{ return FLinearColor(.03, .61, .03, 1); }
	if(Color == EPalette::Green)	{ return FLinearColor(0, .32, .085, 1); }
	if(Color == EPalette::Teal)		{ return FLinearColor(0, .215, .215, 1); }
	if(Color == EPalette::Blue)		{ return FLinearColor(0, .155, .48, 1); }
	if(Color == EPalette::Violet)	{ return FLinearColor(.85, .22, .85, 1); }
	if(Color == EPalette::Purple)	{ return FLinearColor(.46, .01, .84, 1); }
	if(Color == EPalette::Pink)		{ return FLinearColor(1, .07, .3, 1); }
	if(Color == EPalette::Brown)	{ return FLinearColor(.38, .02, .02, 1); }
	if(Color == EPalette::Grey)		{ return FLinearColor(.35, .35, .35, 1); }
	if(Color == EPalette::Black)	{ return FLinearColor(0, 0, 0, 1); }
	if(Color == EPalette::White)	{ return FLinearColor(1, 1, 1, 1); }
	if(Color == EPalette::Transparent)	{ return FLinearColor(0, 0, 0, 0); }

	// Return blank white if invalid enum
	return FLinearColor(1, 1, 1, 1);;
}

/**
* Retrieve the UTexture2D* for a given icon in an IconSet
* @param Icon - the icon to retrieve
* @return the UTexture2D* corresponding to the given item
*/
UTexture2D* UWidgetStudioFunctionLibrary::GetTextureFromIconLibrary(const EIconItem Icon)
{
	UWidgetStudioSubsystem* WF = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	if(!IsValid(WF->GetIconSet())) { return nullptr; }
	
	if (Icon == EIconItem::Attachment)			{ return WF->GetIconSet()->Attachment; }
	if (Icon == EIconItem::ArrowDown) 			{ return WF->GetIconSet()->ArrowDown; }
	if (Icon == EIconItem::ArrowLeft) 			{ return WF->GetIconSet()->ArrowLeft; }
	if (Icon == EIconItem::ArrowLeftRight)		{ return WF->GetIconSet()->ArrowLeftRight; }
	if (Icon == EIconItem::ArrowRight)			{ return WF->GetIconSet()->ArrowRight; }
	if (Icon == EIconItem::ArrowUp)				{ return WF->GetIconSet()->ArrowUp; }
	if (Icon == EIconItem::ArrowInDown)			{ return WF->GetIconSet()->ArrowInDown; }
	if (Icon == EIconItem::ArrowInUp)			{ return WF->GetIconSet()->ArrowInUp; }
	if (Icon == EIconItem::Bookmark) 			{ return WF->GetIconSet()->Bookmark; }
	if (Icon == EIconItem::Calendar) 			{ return WF->GetIconSet()->Calendar; }
	if (Icon == EIconItem::ChevronDown) 		{ return WF->GetIconSet()->ChevronDown; }
	if (Icon == EIconItem::ChevronLeft) 		{ return WF->GetIconSet()->ChevronLeft; }
	if (Icon == EIconItem::ChevronRight)		{ return WF->GetIconSet()->ChevronRight; }
	if (Icon == EIconItem::ChevronUp)			{ return WF->GetIconSet()->ChevronUp; }
	if (Icon == EIconItem::Check)				{ return WF->GetIconSet()->Check; }
	if (Icon == EIconItem::Clipboard)			{ return WF->GetIconSet()->Clipboard; }
	if (Icon == EIconItem::Clock) 				{ return WF->GetIconSet()->Clock; }
	if (Icon == EIconItem::Close) 				{ return WF->GetIconSet()->Close; }
	if (Icon == EIconItem::Cloud) 				{ return WF->GetIconSet()->Cloud; }
	if (Icon == EIconItem::CloudDownload)		{ return WF->GetIconSet()->CloudDownload; }
	if (Icon == EIconItem::CloudUpload)			{ return WF->GetIconSet()->CloudUpload; }
	if (Icon == EIconItem::CogWheel)			{ return WF->GetIconSet()->Cogwheel; }
	if (Icon == EIconItem::Compass)				{ return WF->GetIconSet()->Compass; }
	if (Icon == EIconItem::ControlEnd)			{ return WF->GetIconSet()->ControlEnd; }
	if (Icon == EIconItem::ControlFastForward)	{ return WF->GetIconSet()->ControlFastForward; }
	if (Icon == EIconItem::ControlFullscreen)	{ return WF->GetIconSet()->ControlFullScreen; }
	if (Icon == EIconItem::ControlPause)		{ return WF->GetIconSet()->ControlPause; }
	if (Icon == EIconItem::ControlPlay)			{ return WF->GetIconSet()->ControlPlay; }
	if (Icon == EIconItem::ControlRewind)		{ return WF->GetIconSet()->ControlRewind; }
	if (Icon == EIconItem::ControlStart)		{ return WF->GetIconSet()->ControlStart; }
	if (Icon == EIconItem::ControlStop)			{ return WF->GetIconSet()->ControlStop; }
	if (Icon == EIconItem::Desktop)				{ return WF->GetIconSet()->Desktop; }
	if (Icon == EIconItem::Document)			{ return WF->GetIconSet()->Document; }
	if (Icon == EIconItem::DocumentCopy)		{ return WF->GetIconSet()->DocumentCopy; }
	if (Icon == EIconItem::Download) 			{ return WF->GetIconSet()->Download; }
	if (Icon == EIconItem::Envelope) 			{ return WF->GetIconSet()->Envelope; }
	if (Icon == EIconItem::Exclamation)			{ return WF->GetIconSet()->Exclamation; }
	if (Icon == EIconItem::Expand)				{ return WF->GetIconSet()->Expand; }
	if (Icon == EIconItem::File)				{ return WF->GetIconSet()->File; }
	if (Icon == EIconItem::FileMinus)			{ return WF->GetIconSet()->FileMinus; }
	if (Icon == EIconItem::FilePlus)			{ return WF->GetIconSet()->FilePlus; }
	if (Icon == EIconItem::Folder)				{ return WF->GetIconSet()->Folder; }
	if (Icon == EIconItem::Globe)				{ return WF->GetIconSet()->Globe; }
	if (Icon == EIconItem::GridView)			{ return WF->GetIconSet()->GridView; }
	if (Icon == EIconItem::Hamburger)			{ return WF->GetIconSet()->Hamburger; }
	if (Icon == EIconItem::Heart)				{ return WF->GetIconSet()->Heart; }
	if (Icon == EIconItem::Home) 				{ return WF->GetIconSet()->Home; }
	if (Icon == EIconItem::Info) 				{ return WF->GetIconSet()->Info; }
	if (Icon == EIconItem::Info) 				{ return WF->GetIconSet()->Info; }
	if (Icon == EIconItem::Link) 				{ return WF->GetIconSet()->Link; }
	if (Icon == EIconItem::ListView)			{ return WF->GetIconSet()->ListView; }
	if (Icon == EIconItem::LockClose)			{ return WF->GetIconSet()->LockClose; }
	if (Icon == EIconItem::LockOpen)			{ return WF->GetIconSet()->LockOpen; }
	if (Icon == EIconItem::Magnifier)			{ return WF->GetIconSet()->Magnifier; }
	if (Icon == EIconItem::MagnifierMinus)		{ return WF->GetIconSet()->MagnifierMinus; }
	if (Icon == EIconItem::MagnifierPlus)		{ return WF->GetIconSet()->MagnifierPlus; }
	if (Icon == EIconItem::Map)					{ return WF->GetIconSet()->Map; }
	if (Icon == EIconItem::Microphone)			{ return WF->GetIconSet()->Microphone; }
	if (Icon == EIconItem::Minus)				{ return WF->GetIconSet()->Minus; }
	if (Icon == EIconItem::Mobile)				{ return WF->GetIconSet()->Mobile; }
	if (Icon == EIconItem::Moon)				{ return WF->GetIconSet()->Moon; }
	if (Icon == EIconItem::Navigate)			{ return WF->GetIconSet()->Navigate; }
	if (Icon == EIconItem::Pen)					{ return WF->GetIconSet()->Pen; }
	if (Icon == EIconItem::Phone) 				{ return WF->GetIconSet()->Phone; }
	if (Icon == EIconItem::Photo) 				{ return WF->GetIconSet()->Photo; }
	if (Icon == EIconItem::Pin)					{ return WF->GetIconSet()->Pin; }
	if (Icon == EIconItem::Plus)				{ return WF->GetIconSet()->Plus; }
	if (Icon == EIconItem::Power) 				{ return WF->GetIconSet()->Power; }
	if (Icon == EIconItem::Print) 				{ return WF->GetIconSet()->Print; }
	if (Icon == EIconItem::Question)			{ return WF->GetIconSet()->Question; }
	if (Icon == EIconItem::Refresh)				{ return WF->GetIconSet()->Refresh; }
	if (Icon == EIconItem::ShoppingCart)		{ return WF->GetIconSet()->ShoppingCart; }
	if (Icon == EIconItem::Settings)			{ return WF->GetIconSet()->Settings; }
	if (Icon == EIconItem::Share)				{ return WF->GetIconSet()->Share; }
	if (Icon == EIconItem::ShoppingCart) 		{ return WF->GetIconSet()->ShoppingCart; }
	if (Icon == EIconItem::SpeechBubble) 		{ return WF->GetIconSet()->SpeechBubble; }
	if (Icon == EIconItem::StatisticGraph)		{ return WF->GetIconSet()->StatisticGraph; }
	if (Icon == EIconItem::StatisticPie)		{ return WF->GetIconSet()->StatisticPie; }
	if (Icon == EIconItem::Sun) 				{ return WF->GetIconSet()->Sun; }
	if (Icon == EIconItem::Tag) 				{ return WF->GetIconSet()->Tag; }
	if (Icon == EIconItem::Trash)				{ return WF->GetIconSet()->Trash; }
	if (Icon == EIconItem::Upload)				{ return WF->GetIconSet()->Upload; }
	if (Icon == EIconItem::User)				{ return WF->GetIconSet()->User; }
	if (Icon == EIconItem::Users)				{ return WF->GetIconSet()->Users; }
	if (Icon == EIconItem::Video)				{ return WF->GetIconSet()->Video; }
	if (Icon == EIconItem::Volume)				{ return WF->GetIconSet()->Volume; }
	if (Icon == EIconItem::VolumeMinus)			{ return WF->GetIconSet()->VolumeMinus; }
	if (Icon == EIconItem::VolumeMute) 			{ return WF->GetIconSet()->VolumeMute; }
	if (Icon == EIconItem::VolumePlus) 			{ return WF->GetIconSet()->VolumePlus; }
	if (Icon == EIconItem::Wifi)				{ return WF->GetIconSet()->Wifi; }
	
	return nullptr;
}

/**
* Retrieve the FFontStyle for a given font in a Typography set
* @param FontType - the font type to retrieve
* @return the FFontStyle corresponding to the given EFontType
*/
FFontStyle UWidgetStudioFunctionLibrary::GetTypeScaleFromTypography(const EFontType FontType)
{
	const UWidgetStudioTypography* Typography = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>()->GetTypography();

	if(!IsValid(Typography)) { return FFontStyle(); }
	
	if (FontType == EFontType::H1)				{ return Typography->H1; }
	if (FontType == EFontType::H2)				{ return Typography->H2; }
	if (FontType == EFontType::H3)				{ return Typography->H3; }
	if (FontType == EFontType::H4)				{ return Typography->H4; }
	if (FontType == EFontType::H5)				{ return Typography->H5; }
	if (FontType == EFontType::H6)				{ return Typography->H6; }
	if (FontType == EFontType::Subtitle1)		{ return Typography->Subtitle1; }
	if (FontType == EFontType::Subtitle2)		{ return Typography->Subtitle2; }
	if (FontType == EFontType::Body1)			{ return Typography->Body1; }
	if (FontType == EFontType::Body2)			{ return Typography->Body2; }
	if (FontType == EFontType::Button)			{ return Typography->Button; }
	if (FontType == EFontType::Caption)			{ return Typography->Caption; }
	if (FontType == EFontType::Overline)		{ return Typography->Overline; }

	return FFontStyle();
}

FName UWidgetStudioFunctionLibrary::GetFontWeightName(const EFontWeight FontWeight)
{
	if(FontWeight == EFontWeight::Light)		{ return FName("Light"); }
	if(FontWeight == EFontWeight::SemiLight)	{ return FName("SemiLight"); }
	if(FontWeight == EFontWeight::Regular) 		{ return FName("Regular"); }
	if(FontWeight == EFontWeight::SemiBold) 	{ return FName("SemiBold"); }
	if(FontWeight == EFontWeight::Bold) 		{ return FName("Bold"); }
	if(FontWeight == EFontWeight::Black) 		{ return FName("Black"); }

	return FName("Normal");
}

/**
* Get current control dimensions x and y values as FVector2D
* @return ControlDimensions
*/
FVector2D UWidgetStudioFunctionLibrary::GetControlDimensions()
{
	UWidgetStudioSubsystem* WF = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	if (IsValid(WF))
	{
		return WF->GetControlDimensions();
	}
	
	return FVector2D(35, 325);
}

FLinearColor UWidgetStudioFunctionLibrary::GetColorBasedValue(const float ValuePercent, const bool bReverseOrder)
{
	if (ValuePercent >= 1.f) { return GetColorFromPalette(bReverseOrder ? EPalette::Green : EPalette::Red); }
	if (ValuePercent >= 0.75f) { return GetColorFromPalette(bReverseOrder ? EPalette::Olive : EPalette::Orange); }
	if (ValuePercent >= 0.50f) { return GetColorFromPalette(EPalette::Yellow); }
	if (ValuePercent >= 0.25f) { return GetColorFromPalette(bReverseOrder ? EPalette::Orange : EPalette::Olive); }
	if (ValuePercent >= 0.f) { return GetColorFromPalette(bReverseOrder ? EPalette::Red : EPalette::Green); }

	return GetColorFromPalette(EPalette::Green);
}

EPalette UWidgetStudioFunctionLibrary::GetPaletteBasedValue(const float ValuePercent, const bool bReverseOrder)
{
	if (ValuePercent >= 1.f) { return bReverseOrder ? EPalette::Green : EPalette::Red; }
	if (ValuePercent >= 0.75f) { return bReverseOrder ? EPalette::Olive : EPalette::Orange; }
	if (ValuePercent >= 0.50f) { return EPalette::Yellow; }
	if (ValuePercent >= 0.25f) { return bReverseOrder ? EPalette::Orange : EPalette::Olive; }
	if (ValuePercent >= 0.f) { return bReverseOrder ? EPalette::Red : EPalette::Green; }
	
	return EPalette::Green;
}


void UWidgetStudioFunctionLibrary::InterpWidgetColor(UUserWidget* Widget, const EPalette TargetColor, const float Speed)
{
	// Exit if the widget isn't valid.
	if (!IsValid(Widget)) { return; }

	const FLinearColor NewColor = GetColorFromPalette(TargetColor);

	// Smoothly lerp to the new color.
	if (Widget->ColorAndOpacity != NewColor)
	{
		Widget->SetColorAndOpacity(
			UKismetMathLibrary::CInterpTo(
				Widget->ColorAndOpacity,
				NewColor,
				Widget->GetWorld()->GetDeltaSeconds(),
				Speed
			)
		);
	}
}

void UWidgetStudioFunctionLibrary::InterpImageColor(UImage* Widget, const EPalette TargetColor, const float Speed)
{
	// Exit if the widget isn't valid.
	if (!IsValid(Widget)) { return; }

	const FLinearColor NewColor = GetColorFromPalette(TargetColor);

	// Smoothly lerp to the new color.
	if (Widget->ColorAndOpacity != NewColor)
	{
		Widget->SetColorAndOpacity(
			UKismetMathLibrary::CInterpTo(
				Widget->ColorAndOpacity,
				NewColor,
				Widget->GetWorld()->GetDeltaSeconds(),
				Speed
			)
		);
	}
}

void UWidgetStudioFunctionLibrary::InterpTextColor(UTextBlock* Widget, const EPalette TargetColor, const float Speed)
{
	// Exit if the widget isn't valid.
	if (!IsValid(Widget)) { return; }

	
	const FLinearColor NewColor = GetColorFromPalette(TargetColor);
	const FLinearColor CurrentColor = Widget->ColorAndOpacity.GetSpecifiedColor();

	if (CurrentColor != NewColor)
	{
		Widget->SetColorAndOpacity(
			FSlateColor(
				UKismetMathLibrary::CInterpTo(
					CurrentColor,
					NewColor,
					Widget->GetWorld()->GetDeltaSeconds(),
					Speed
				)
			)
		);
	}
}

void UWidgetStudioFunctionLibrary::InterpEditableTextBoxColor(UEditableTextBox* Widget, const EPalette TargetColor,
                                                             const float Speed)
{
	// Exit if the widget isn't valid.
	if (!IsValid(Widget)) { return; }
	
	const FLinearColor NewColor = GetColorFromPalette(TargetColor);
	const FLinearColor CurrentColor = Widget->WidgetStyle.ForegroundColor.GetSpecifiedColor();

	if (CurrentColor != NewColor)
	{
		Widget->WidgetStyle.SetForegroundColor(
			UKismetMathLibrary::CInterpTo(
				CurrentColor,
				NewColor,
				Widget->GetWorld()->GetDeltaSeconds(),
				Speed
			)
		);
	}
}

void UWidgetStudioFunctionLibrary::InterpSizeBoxOverrides(USizeBox* Widget, const float TargetWidth, const float TargetHeight,
                                                         const float Speed, const bool bIgnoreZero)
{
	// Exit if the widget isn't valid.
	if (!IsValid(Widget)) { return; }
	
	// Smoothly lerp to the target height.
	if (Widget->HeightOverride != TargetHeight && (TargetHeight != 0 || bIgnoreZero == false))
	{
		Widget->SetHeightOverride(
			UKismetMathLibrary::FInterpTo(
				Widget->HeightOverride,
				TargetHeight,
				Widget->GetWorld()->GetDeltaSeconds(),
				Speed
			)
		);
	}

	// Smoothly lerp to the target width.
	if (Widget->WidthOverride != TargetWidth && (TargetWidth != 0 || bIgnoreZero == false))
	{
		Widget->SetWidthOverride(
			UKismetMathLibrary::FInterpTo(
				Widget->WidthOverride,
				TargetWidth,
				Widget->GetWorld()->GetDeltaSeconds(),
				Speed
			)
		);
	}
}

void UWidgetStudioFunctionLibrary::InterpSizeBoxMinOverrides(USizeBox* Widget, const float TargetWidth, const float TargetHeight,
                                                            const float Speed)
{
	// Exit if the widget isn't valid.
	if (!IsValid(Widget)) { return; }
	
	// Smoothly lerp to the target height.
	if (Widget->MinDesiredHeight != TargetHeight && TargetHeight != 0)
	{
		Widget->SetMinDesiredHeight(
			UKismetMathLibrary::FInterpTo(
				Widget->MinDesiredHeight,
				TargetHeight,
				Widget->GetWorld()->GetDeltaSeconds(),
				Speed
			)
		);
	}

	// Smoothly lerp to the target width.
	if (Widget->MinDesiredWidth != TargetWidth && TargetWidth != 0)
	{
		Widget->SetMinDesiredWidth(
			UKismetMathLibrary::FInterpTo(
				Widget->MinDesiredWidth,
				TargetWidth,
				Widget->GetWorld()->GetDeltaSeconds(),
				Speed
			)
		);
	}
}

void UWidgetStudioFunctionLibrary::InterpWidgetTranslation(UWidget* Widget, const FVector2D TargetTranslation,
                                                          const float Speed)
{
	// Exit if the widget isn't valid.
	if (!IsValid(Widget)) { return; }
	
	if (Widget->RenderTransform.Translation != TargetTranslation)
	{
		Widget->SetRenderTranslation(
			UKismetMathLibrary::Vector2DInterpTo(
				Widget->RenderTransform.Translation,
				TargetTranslation,
				Widget->GetWorld()->GetDeltaSeconds(),
				Speed
			)
		);
	}
}

void UWidgetStudioFunctionLibrary::InterpWidgetScale(UWidget* Widget, const FVector2D TargetScale, const float Speed)
{
	// Exit if the widget isn't valid.
	if (!IsValid(Widget)) { return; }
	
	if (Widget->RenderTransform.Scale != TargetScale)
	{
		Widget->SetRenderScale(
			UKismetMathLibrary::Vector2DInterpTo(
				Widget->RenderTransform.Scale,
				TargetScale,
				Widget->GetWorld()->GetDeltaSeconds(),
				Speed
			)
		);
	}
}

void UWidgetStudioFunctionLibrary::InterpWidgetRotation(UWidget* Widget, const float TargetRotation, const float Speed)
{
	// Exit if the widget isn't valid.
	if (!IsValid(Widget)) { return; }
	
	if (Widget->GetRenderTransformAngle() != TargetRotation)
	{
		Widget->SetRenderTransformAngle(
			UKismetMathLibrary::FInterpTo(
				Widget->GetRenderTransformAngle(),
				TargetRotation,
				Widget->GetWorld()->GetDeltaSeconds(),
				Speed
			)
		);
	}
}

void UWidgetStudioFunctionLibrary::InterpWidgetOpacity(UWidget* Widget, const float TargetOpacity, const float Speed)
{
	// Exit if the widget isn't valid.
	if (!IsValid(Widget)) { return; }
	
	if (Widget->RenderOpacity != TargetOpacity)
	{
		Widget->SetRenderOpacity(
			UKismetMathLibrary::FInterpTo(
				Widget->RenderOpacity,
				TargetOpacity,
				Widget->GetWorld()->GetDeltaSeconds(),
				Speed
			)
		);
	}
}

void UWidgetStudioFunctionLibrary::InterpBrushImageSize(UImage* Image, const float TargetWidth, const float TargetHeight,
                                                       const float Speed)
{
	// Exit if the widget isn't valid.
	if (!IsValid(Image)) { return; }
	
	const FVector2D NewSize = FVector2D(TargetWidth, TargetHeight);
	if (!Image->Brush.ImageSize.Equals(NewSize))
	{
		// UE4 Implementation
		Image->SetBrushSize(UKismetMathLibrary::Vector2DInterpTo(
			Image->Brush.ImageSize,
			NewSize,
			Image->GetWorld()->GetDeltaSeconds(),
			Speed
			)
		);

		// UE5 Implementation
		Image->Brush.SetImageSize(
			UKismetMathLibrary::Vector2DInterpTo(
				Image->Brush.ImageSize,
				NewSize,
				Image->GetWorld()->GetDeltaSeconds(),
				Speed
			)
		);
	}
}

UMaterialInterface* UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial()
{
	const FName BackgroundMaterialPath = TEXT("/WidgetStudio/Shared/Materials/M_UI_RoundedSquare.M_UI_RoundedSquare");
	return LoadMaterialFromPath(BackgroundMaterialPath);
}

UMaterialInterface* UWidgetStudioFunctionLibrary::GetRoundedOutlineMaterial()
{
	const FName BackgroundMaterialPath = TEXT("/WidgetStudio/Shared/Materials/M_UI_RoundedSquare_Outline.M_UI_RoundedSquare_Outline");
	return LoadMaterialFromPath(BackgroundMaterialPath);
}

UMaterialInterface* UWidgetStudioFunctionLibrary::GetRoundedShadowMaterial()
{
	const FName BackgroundMaterialPath = TEXT("/WidgetStudio/Shared/Materials/M_UI_RoundedDropShadow.M_UI_RoundedDropShadow");
	return LoadMaterialFromPath(BackgroundMaterialPath);
}

UFont* UWidgetStudioFunctionLibrary::GetTypefaceFromTypography()
{
	UWidgetStudioTypography* Typography = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>()->GetTypography();

	if (IsValid(Typography))
	{
		return Typography->Typeface;
	}

	else
	{
		return LoadObject<UFont>(nullptr, TEXT("/Engine/EngineFonts/Roboto.Roboto"));
	}
}

bool UWidgetStudioFunctionLibrary::ContainsNumbers(const FText InText)
{
	const FString TempString = InText.ToString();

	const bool IsNumeric = TempString.IsNumeric();
	const bool ContainsZero = TempString.Contains("0");
	const bool ContainsOne = TempString.Contains("1");
	const bool ContainsTwo = TempString.Contains("2");
	const bool ContainsThree = TempString.Contains("3");
	const bool ContainsFour = TempString.Contains("4");
	const bool ContainsFive = TempString.Contains("5");
	const bool ContainsSix = TempString.Contains("6");
	const bool ContainsSeven = TempString.Contains("7");
	const bool ContainsEight = TempString.Contains("8");
	const bool ContainsNine = TempString.Contains("9");

	if ( IsNumeric || ContainsZero || ContainsOne || ContainsTwo || ContainsThree || ContainsFour ||
		ContainsFive || ContainsSix || ContainsSeven || ContainsEight | ContainsNine)
	{
		return true;
	}

	return false;
}

FText UWidgetStudioFunctionLibrary::FilterByCharacterLimit(const FText InText, const int32 CharacterLimit)
{
	if (CharacterLimit <= 0) { return InText; }

	return FText::FromString(InText.ToString().TrimStartAndEnd().Left(CharacterLimit));;
}

FSlateFontInfo UWidgetStudioFunctionLibrary::ConstructFontInfoFromTypography(const EFontType FontType)
{
	const FFontStyle LabelFontStyle = GetTypeScaleFromTypography(FontType);
	
	FSlateFontInfo LabelFontInfo;
	LabelFontInfo.FontObject = GetTypefaceFromTypography();
	LabelFontInfo.Size = LabelFontStyle.Size;
	LabelFontInfo.TypefaceFontName = GetFontWeightName(LabelFontStyle.Weight);
	LabelFontInfo.LetterSpacing = LabelFontStyle.LetterSpacing;
	
	return LabelFontInfo;
}

FSlateFontInfo UWidgetStudioFunctionLibrary::ConstructFontInfoFromStyle(const FFontStyle FontStyle)
{
	FSlateFontInfo LabelFontInfo;
	LabelFontInfo.FontObject = GetTypefaceFromTypography();
	LabelFontInfo.Size = FontStyle.Size;
	LabelFontInfo.TypefaceFontName = GetFontWeightName(FontStyle.Weight);
	LabelFontInfo.LetterSpacing = FontStyle.LetterSpacing;
	
	return LabelFontInfo;
}

float UWidgetStudioFunctionLibrary::GetSizeByModifier(const ESizeModifier Modifier, const float InSize)
{
	if (Modifier == ESizeModifier::Mini) { return InSize * 0.55f; }
	if (Modifier == ESizeModifier::Tiny) { return InSize * 0.7f; }
	if (Modifier == ESizeModifier::Small) { return InSize * 0.85f; }
	if (Modifier == ESizeModifier::Regular) { return InSize * 1.f; }
	if (Modifier == ESizeModifier::Large) { return InSize * 1.15f; }
	if (Modifier == ESizeModifier::Big) { return InSize * 1.3f; }
	if (Modifier == ESizeModifier::Huge) { return InSize * 1.45f; }
	if (Modifier == ESizeModifier::Massive) { return InSize * 1.6f; }

	return InSize;
}

float UWidgetStudioFunctionLibrary::GetBorderRadius()
{
	return GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>()->BorderRadius;
}

UWidgetStudioTheme* UWidgetStudioFunctionLibrary::GetCurrentTheme()
{
	UWidgetStudioSubsystem* WF = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	return WF->GetTheme();
}

UWidgetStudioTypography* UWidgetStudioFunctionLibrary::GetCurrentTypography()
{
	UWidgetStudioSubsystem* WF = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	return WF->GetTypography();
}

UWidgetStudioIconSet* UWidgetStudioFunctionLibrary::GetCurrentIconSet()
{
	UWidgetStudioSubsystem* WF = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	return WF->GetIconSet();
}

bool UWidgetStudioFunctionLibrary::SetTheme(const TSoftObjectPtr<UWidgetStudioTheme> NewTheme)
{
	UWidgetStudioSubsystem* WF = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	return WF->SetTheme(NewTheme);
}

bool UWidgetStudioFunctionLibrary::SetTypography(const TSoftObjectPtr<UWidgetStudioTypography> NewTypography)
{
	UWidgetStudioSubsystem* WF = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	return WF->SetTypography(NewTypography);
}

bool UWidgetStudioFunctionLibrary::SetIconSet(const TSoftObjectPtr<UWidgetStudioIconSet> NewIconSet)
{
	UWidgetStudioSubsystem* WF = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	return WF->SetIconSet(NewIconSet);
}
