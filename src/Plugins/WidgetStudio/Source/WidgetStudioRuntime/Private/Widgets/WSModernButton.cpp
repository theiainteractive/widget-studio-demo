// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WSModernButton.h"

#include "WSFunctionLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/OverlaySlot.h"
#include "Components/ScaleBoxSlot.h"
#include "Components/SizeBoxSlot.h"
#include "Components/VerticalBoxSlot.h"

#define LOCTEXT_NAMESPACE "WidgetStudio"

#if WITH_EDITOR
const FText UWidgetStudioModernButton::GetPaletteCategory()
{
	return LOCTEXT("WidgetStudio", "*Widget Studio - Modern");
}
#endif

#undef LOCTEXT_NAMESPACE

TSharedRef<SWidget> UWidgetStudioModernButton::RebuildWidget()
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
		* --- Background Scale Box
		* ---- Background Overlay
		* ----- DropShadow
		* ----- Background
		* --- Content Overlay
		* ---- Horizontal Content // Vertical Content (Interchangeable)
		* ----- Icon Scale Box
		* ------ Icon Item
		* ----- Spacer Item
		* ----- Text Item
		*/
		
		/* Construct Widgets */
		SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Size Box"));
		Overlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Overlay"));
		BackgroundOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Background Overlay"));
		BackgroundScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Background Scale Box"));
		DropShadow = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("DropShadow"));
		Background = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Background"));
		ContentOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Content Overlay"));
		HorizontalContent = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("HContent"));
		VerticalContent = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("VContent"));
		IconItem = WidgetTree->ConstructWidget<UWidgetStudioIcon>(UWidgetStudioIcon::StaticClass(), TEXT("Icon Item"));
		SpacerItem = WidgetTree->ConstructWidget<USpacer>(USpacer::StaticClass(), TEXT("Spacer Item"));
		TextItem = WidgetTree->ConstructWidget<UWidgetStudioText>(UWidgetStudioText::StaticClass(), TEXT("Text Item"));
		
				
		/* Construct Hierarchy */
		RootWidget->AddChild(SizeBox);
		SizeBox->AddChild(Overlay);

		Overlay->AddChild(BackgroundScaleBox);
		BackgroundScaleBox->AddChild(BackgroundOverlay);
		BackgroundOverlay->AddChild(DropShadow);
		BackgroundOverlay->AddChild(Background);

		Overlay->AddChild(ContentOverlay);
		ContentOverlay->AddChild(HorizontalContent);
		ContentOverlay->AddChild(VerticalContent);
		ContentOverlay->AddChild(IconItem);
		ContentOverlay->AddChild(TextItem);
		
		InitializeStyling();
	}

	return Widget;
}

int32 UWidgetStudioModernButton::NativePaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	if (!bDisablePainting)
	{
		/* Smoothly update opacity based on hover state. */
		if (ButtonStyle == EButtonStyle::Content || ButtonStyle == EButtonStyle::Line)
		{
			UWidgetStudioFunctionLibrary::InterpWidgetOpacity(Overlay, IsHovered() ? 1.f : 0.75f, AnimationTime);
		}
		else
		{
			UWidgetStudioFunctionLibrary::InterpWidgetOpacity(BackgroundOverlay, IsHovered() ? 1.f : 0.85f, AnimationTime);
		}
		
		/* Smoothly update opacity based on hover state. */
		UWidgetStudioFunctionLibrary::InterpWidgetScale(Overlay, IsPressed() ? FVector2D(0.95f, 0.95f) : FVector2D(1, 1), AnimationTime * 4);

		/* Smoothly update size */
		UWidgetStudioFunctionLibrary::InterpSizeBoxMinOverrides(SizeBox, ContentStyle == EButtonContentStyle::Icon ? GetDimensions().Y : GetDimensions().X, GetDimensions().Y, 0);
		
		/* Smoothly update DropShadow opacity */
		const float NewDropShadowOpacity = bEnableDropShadow && ButtonStyle == EButtonStyle::Solid ? IsHovered() ? 0.5f : 0.35f : 0.f;
		UWidgetStudioFunctionLibrary::InterpWidgetOpacity(DropShadow, NewDropShadowOpacity, AnimationTime);
		
		/* Smoothly update DropShadow location */
		const float NewDropShadowLoc = ButtonStyle == EButtonStyle::Line ? 1.f : IsHovered() ? 5.f : 1.f;
		UWidgetStudioFunctionLibrary::InterpWidgetTranslation(DropShadow, FVector2D(0, NewDropShadowLoc), AnimationTime);
			
		/* Smoothly update background color */
		const EPalette TargetBackgroundColor = IsChecked() ? GetCheckedBackgroundColor() : GetStandardBackgroundColor();
		UWidgetStudioFunctionLibrary::InterpImageColor(Background, TargetBackgroundColor, AnimationTime * 3);
		
		/* Smoothly lerp the content color based on current states and properties */
		const EPalette TargetContentColor = IsChecked() ? ButtonStyle == EButtonStyle::Solid ? GetCheckedContentColor() :
			GetCheckedBackgroundColor() : ButtonStyle == EButtonStyle::Solid ? GetStandardContentColor() : GetStandardBackgroundColor();
		UWidgetStudioFunctionLibrary::InterpWidgetColor(TextItem, TargetContentColor, AnimationTime * 3);
		
		if (bColorIconByPalette)
		{
			UWidgetStudioFunctionLibrary::InterpWidgetColor(IconItem, TargetContentColor, AnimationTime * 3);
		}
	}	
	return LayerId;
}

void UWidgetStudioModernButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	
	UpdateStyling();
	UpdateContentPositionsAndStyling();

	if (ContentOverlay)
	{
		Cast<UOverlaySlot>(ContentOverlay->Slot)->SetHorizontalAlignment(ContentAlignment);
	}

	if (IconItem)
	{
		if (IsUsingCustomIcon()) { IconItem->SetCustomIcon(CustomIcon); }
		else { IconItem->SetIcon(Icon); }
		IconItem->SetSize(GetDimensions().Y / 3);
		IconItem->SetVisibility(ContentStyle == EButtonContentStyle::Label ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

		if (!bColorIconByPalette)
		{
			IconItem->SetColor(EPalette::White);
		}
	}

	if (SpacerItem)
	{
		SpacerItem->SetVisibility(ContentStyle ==  EButtonContentStyle::IconWithLabel ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (TextItem)
	{
		TextItem->SetText(Text);
		TextItem->SizeModifier = SizeModifier;
	
		if (ContentAlignment == HAlign_Left) { TextItem->SetJustification(ETextJustify::Left); }
		if (ContentAlignment == HAlign_Center) { TextItem->SetJustification(ETextJustify::Center); }
		if (ContentAlignment == HAlign_Right) { TextItem->SetJustification(ETextJustify::Right); }
		if (ContentAlignment == HAlign_Fill) { TextItem->SetJustification(ETextJustify::Center); }

		TextItem->SetVisibility(ContentStyle == EButtonContentStyle::Icon ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	}
}

void UWidgetStudioModernButton::InitializeStyling()
{
	// Setup Size Box
	SizeBox->SetMinDesiredWidth(GetDimensions().X);
	SizeBox->SetMinDesiredHeight(GetDimensions().Y);
	Cast<UOverlaySlot>(SizeBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(SizeBox->Slot)->SetVerticalAlignment(VAlign_Fill);

	// Setup Overlay
	Cast<USizeBoxSlot>(Overlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<USizeBoxSlot>(Overlay->Slot)->SetVerticalAlignment(VAlign_Fill);

	// Setup Background
	Background->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(IsChecked() ? GetCheckedBackgroundColor() : GetStandardBackgroundColor()));
	Background->SetRenderOpacity(ButtonStyle == EButtonStyle::Content ? 0.f : 1.f);
	DropShadow->SetRenderOpacity(BaseDropShadowOpacity);

	// Setup Text
	TextItem->bDisablePainting = false;
	TextItem->SetColor(EPalette::White);
	TextItem->SetType(EFontType::Button);
		
	// Setup Icon
	IconItem->bDisablePainting = true;
	IconItem->SetColor(EPalette::White);
	IconItem->SetSize(GetDimensions().Y / 3);

	// Apply Colors
	const EPalette TargetContentColor = IsChecked() ? ButtonStyle == EButtonStyle::Solid ? GetCheckedContentColor() :
		GetCheckedBackgroundColor() : ButtonStyle == EButtonStyle::Solid ? GetStandardContentColor() : GetStandardBackgroundColor();

	TextItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(TargetContentColor));
	if (bColorIconByPalette)
	{
		IconItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(TargetContentColor));
	}
		
	const EPalette TargetBackgroundColor = IsChecked() ? GetCheckedBackgroundColor() : GetStandardBackgroundColor();
	Background->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(TargetBackgroundColor));
}

void UWidgetStudioModernButton::UpdateStyling()
{
	const float Radius = UWidgetStudioFunctionLibrary::GetBorderRadius();
	const float RoundedSize = CornerStyle == ECornerStyle::Pill ? ContentStyle == EButtonContentStyle::Icon ? GetDimensions().Y + Radius : GetDimensions().Y : CornerStyle == ECornerStyle::Rounded ? FMath::Clamp(Radius, 0.f, GetDimensions().Y) : 0;

	/* Setup Overlay */
	if (ContentOverlay)
	{
		Cast<UOverlaySlot>(ContentOverlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(ContentOverlay->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	/* Setup Background Scale Box */
	if (BackgroundScaleBox)
	{
		BackgroundScaleBox->SetStretch(EStretch::Fill);
		Cast<UOverlaySlot>(BackgroundScaleBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(BackgroundScaleBox->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<UOverlaySlot>(BackgroundScaleBox->Slot)->SetPadding(0);
	}
	
	/* Setup Background Overlay */
	if (BackgroundOverlay)
	{
		Cast<UScaleBoxSlot>(BackgroundOverlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UScaleBoxSlot>(BackgroundOverlay->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	/* Setup DropShadow */
	if (DropShadow)
	{
		DropShadow->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedShadowMaterial());
		DropShadow->SetBrushSize(FVector2D(RoundedSize, RoundedSize));
		DropShadow->SetColorAndOpacity(FLinearColor::Black);
		DropShadow->SetRenderOpacity(bEnableDropShadow ? 0.35f : 0.f);
		DropShadow->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		DropShadow->Brush.DrawAs = ESlateBrushDrawType::Box;
		DropShadow->Brush.SetImageSize(FVector2D(RoundedSize, RoundedSize));

		if (ButtonStyle == EButtonStyle::Content) { DropShadow->SetRenderOpacity(0); }
		Cast<UOverlaySlot>(DropShadow->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(DropShadow->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<UOverlaySlot>(DropShadow->Slot)->SetPadding(FMargin(1, 1, 1, 0));
	}
	
	/* Setup Background */
	if (Background)
	{
		Background->SetBrushFromMaterial(ButtonStyle == EButtonStyle::Line ? UWidgetStudioFunctionLibrary::GetRoundedOutlineMaterial() : UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
		Background->SetBrushSize(FVector2D(RoundedSize, RoundedSize));
		Background->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		Background->Brush.DrawAs = ESlateBrushDrawType::Box;
		Background->Brush.SetImageSize(FVector2D(RoundedSize, RoundedSize));
		if (ButtonStyle == EButtonStyle::Content) { Background->SetRenderOpacity(0); }
		Cast<UOverlaySlot>(Background->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(Background->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	/* Setup Content Overlay */
	if (ContentOverlay)
	{
		Cast<UOverlaySlot>(ContentOverlay->Slot)->SetVerticalAlignment(VAlign_Center);
	}
	
	/* Horizontal Box Setup */
	if (HorizontalContent)
	{
		Cast<UOverlaySlot>(HorizontalContent->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(HorizontalContent->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<UOverlaySlot>(HorizontalContent->Slot)->SetPadding(
			FVector4(
				ContentStyle == EButtonContentStyle::Label || ContentStyle == EButtonContentStyle::IconWithLabel ? ContentPadding * 3 : ContentPadding,
				ContentStyle == EButtonContentStyle::Icon ? ContentPadding : 0,
				ContentStyle == EButtonContentStyle::Label || ContentStyle == EButtonContentStyle::IconWithLabel ? ContentPadding * 3 : ContentPadding,
				ContentStyle == EButtonContentStyle::Icon ? ContentPadding : 0
			)
		);
	}

	/* Vertical Box Setup */
	if (VerticalContent)
	{
		Cast<UOverlaySlot>(VerticalContent->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(VerticalContent->Slot)->SetVerticalAlignment(VAlign_Center);
		Cast<UOverlaySlot>(VerticalContent->Slot)->SetPadding(
			FVector4(
				ContentStyle == EButtonContentStyle::Label || ContentStyle == EButtonContentStyle::IconWithLabel ? ContentPadding * 2 : ContentPadding,
				ContentStyle == EButtonContentStyle::Icon ? ContentPadding : 10,
				ContentStyle == EButtonContentStyle::Label || ContentStyle == EButtonContentStyle::IconWithLabel ? ContentPadding * 2 : ContentPadding,
				ContentStyle == EButtonContentStyle::Icon ? ContentPadding : 10
			)
		);
	}
}

void UWidgetStudioModernButton::UpdateContentPositionsAndStyling() const
{
	const bool bIsHorizontal = GetIconPlacement() == EIconPlacement::Left || GetIconPlacement() == EIconPlacement::Right;
	if (bIsHorizontal)
	{
		HorizontalContent->SetVisibility(ESlateVisibility::HitTestInvisible);
		VerticalContent->SetVisibility(ESlateVisibility::Collapsed);

		if (GetIconPlacement() == EIconPlacement::Left)
		{
			HorizontalContent->AddChild(IconItem);
			HorizontalContent->AddChild(SpacerItem);
			HorizontalContent->AddChild(TextItem);
		}
		else
		{
			HorizontalContent->AddChild(TextItem);
			HorizontalContent->AddChild(SpacerItem);
			HorizontalContent->AddChild(IconItem);
		}

		/* Icon */
		Cast<UHorizontalBoxSlot>(IconItem->Slot)->SetHorizontalAlignment(HAlign_Center);
		Cast<UHorizontalBoxSlot>(IconItem->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<UHorizontalBoxSlot>(IconItem->Slot)->SetSize(ESlateSizeRule::Automatic);
		Cast<UHorizontalBoxSlot>(IconItem->Slot)->SetPadding(5);

		/* Spacer*/
		SpacerItem->SetSize(FVector2D(5, 0));

		/* Text */
		Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetHorizontalAlignment(HAlign_Center);
		Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetVerticalAlignment(VAlign_Center);
		Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetSize(ESlateSizeRule::Fill);
	}
	else
	{		
		HorizontalContent->SetVisibility(ESlateVisibility::Collapsed);
		VerticalContent->SetVisibility(ESlateVisibility::HitTestInvisible);
		if (GetIconPlacement() == EIconPlacement::Top)
		{
			VerticalContent->AddChild(IconItem);
			VerticalContent->AddChild(SpacerItem);
			VerticalContent->AddChild(TextItem);

			/* Icon */
			Cast<UVerticalBoxSlot>(IconItem->Slot)->SetVerticalAlignment(VAlign_Fill);
		}
		else
		{
			VerticalContent->AddChild(TextItem);
			VerticalContent->AddChild(SpacerItem);
			VerticalContent->AddChild(IconItem);

			/* Icon */
			Cast<UVerticalBoxSlot>(IconItem->Slot)->SetVerticalAlignment(VAlign_Bottom);
		}

		/* Icon */
		Cast<UVerticalBoxSlot>(IconItem->Slot)->SetHorizontalAlignment(HAlign_Center);
		Cast<UVerticalBoxSlot>(IconItem->Slot)->SetSize(ESlateSizeRule::Automatic);
		Cast<UVerticalBoxSlot>(IconItem->Slot)->SetPadding(5);
		
		/* Spacer */
		SpacerItem->SetSize(FVector2D(0, 0));
		
		/* Text */
		Cast<UVerticalBoxSlot>(TextItem->Slot)->SetHorizontalAlignment(HAlign_Center);
		Cast<UVerticalBoxSlot>(TextItem->Slot)->SetVerticalAlignment(VAlign_Center);
		Cast<UVerticalBoxSlot>(TextItem->Slot)->SetSize(ESlateSizeRule::Fill);
	}
}

bool UWidgetStudioModernButton::IsTextValid() const
{
	return !Text.IsEmpty();
}

FText UWidgetStudioModernButton::GetText() const
{
	return Text;
}

bool UWidgetStudioModernButton::IsIconValid() const
{
	return UWidgetStudioFunctionLibrary::GetTextureFromIconLibrary(Icon) != nullptr;
}

EIconItem UWidgetStudioModernButton::GetIcon() const
{
	return Icon;
}

bool UWidgetStudioModernButton::IsIconColoredByPalette() const
{
	return bColorIconByPalette;
}

void UWidgetStudioModernButton::SetColorIconByPalette(const bool NewState)
{
	bColorIconByPalette = NewState;
	SynchronizeProperties();
}

bool UWidgetStudioModernButton::IsUsingCustomIcon() const
{
	return CustomIcon != nullptr;
}

UTexture2D* UWidgetStudioModernButton::GetCustomIcon() const
{
	return CustomIcon;
}

void UWidgetStudioModernButton::SetCustomIcon(UTexture2D* NewCustomIcon)
{
	CustomIcon = NewCustomIcon;
	SynchronizeProperties();
}

void UWidgetStudioModernButton::ClearCustomIcon()
{
	CustomIcon = nullptr;
	IconItem->ClearCustomIcon();
	SynchronizeProperties();
}

void UWidgetStudioModernButton::SetText(const FText NewText)
{
	Text = NewText;
	SynchronizeProperties();
}

void UWidgetStudioModernButton::SetAlignment(const TEnumAsByte<EHorizontalAlignment> NewAlignment)
{
	ContentAlignment = NewAlignment;
	SynchronizeProperties();
}

void UWidgetStudioModernButton::SetIcon(const EIconItem NewIcon)
{
	Icon = NewIcon;
	SynchronizeProperties();
}

void UWidgetStudioModernButton::SetIconPlacement(const EIconPlacement NewIconPlacement)
{
	IconPlacement = NewIconPlacement;
	SynchronizeProperties();
}

void UWidgetStudioModernButton::SetStandardBackgroundColor(const EPalette NewColor)
{
	StandardBackgroundColor = NewColor;
}

void UWidgetStudioModernButton::SetCheckedBackgroundColor(const EPalette NewColor)
{
	CheckedBackgroundColor = NewColor;
}

void UWidgetStudioModernButton::SetStandardContentColor(const EPalette NewColor)
{
	StandardContentColor = NewColor;
}

void UWidgetStudioModernButton::SetCheckedContentColor(const EPalette NewColor)
{
	CheckedContentColor = NewColor;
}

void UWidgetStudioModernButton::SetCornerStyle(const ECornerStyle NewCornerStyle)
{
	CornerStyle = NewCornerStyle;
	SynchronizeProperties();
}

EPalette UWidgetStudioModernButton::GetStandardBackgroundColor() const
{
	return StandardBackgroundColor;
}

EPalette UWidgetStudioModernButton::GetCheckedBackgroundColor() const
{
	return CheckedBackgroundColor;
}

EPalette UWidgetStudioModernButton::GetStandardContentColor() const
{
	return StandardContentColor;
}

EPalette UWidgetStudioModernButton::GetCheckedContentColor() const
{
	return CheckedContentColor;
}

EIconPlacement UWidgetStudioModernButton::GetIconPlacement() const
{
	return IconPlacement;
}

ECornerStyle UWidgetStudioModernButton::GetCornerStyle() const
{
	return CornerStyle;
}

EButtonStyle UWidgetStudioModernButton::GetButtonStyle() const
{
	return ButtonStyle;
}

EButtonContentStyle UWidgetStudioModernButton::GetContentStyle() const
{
	return ContentStyle;
}

void UWidgetStudioModernButton::SetButtonStyle(const EButtonStyle NewButtonStyle)
{
	ButtonStyle = NewButtonStyle;
	SynchronizeProperties();
}

void UWidgetStudioModernButton::SetContentStyle(const EButtonContentStyle NewContentStyle)
{
	ContentStyle = NewContentStyle;
	SynchronizeProperties();
}
