// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WSModernCheckBox.h"

#include "Components/PanelWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/OverlaySlot.h"
#include "Components/ScaleBoxSlot.h"
#include "Components/SizeBoxSlot.h"

#define LOCTEXT_NAMESPACE "WidgetStudio"

#if WITH_EDITOR
const FText UWidgetStudioModernCheckBox::GetPaletteCategory()
{
	return LOCTEXT("WidgetStudio", "*Widget Studio - Modern");
}
#endif

#undef LOCTEXT_NAMESPACE

TSharedRef<SWidget> UWidgetStudioModernCheckBox::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;
	
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{

		/*
		 * RootWidget (RetainerBox)
		 * - Size Box
		 * -- Overlay
		 * --- Background Scale Box
		 * ---- Background Overlay
		 * ----- DropShadow
		 * ----- Background
		 * ----- Background Outline
		 * --- HBox
		 * ---- Check Scale Box
		 * ----- Check Overlay
		 * ------ Check Background
		 * ------ Check Outline
		 * ------ Icon Item
		 * ---- Spacer Item
		 * ---- Text Item
		 */
		
		// Construct Widgets
		Overlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Overlay"));
		SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("SizeBox"));
		BackgroundScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Background Scale Box"));
		BackgroundOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Background Overlay"));
		DropShadow = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("DropDown"));
		Background = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Background"));
		BackgroundOutline = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Background Outline"));
		CheckScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Check Scale Box"));
		HBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("HBox"));
		CheckOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Check Overlay"));
		CheckBackground = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Check Background Item"));
		CheckOutline = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Check Outline Item"));
		IconItem = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Icon Item"));
		TextItem = WidgetTree->ConstructWidget<UWidgetStudioText>(UWidgetStudioText::StaticClass(), TEXT("Text Item"));
		SpacerItem = WidgetTree->ConstructWidget<USpacer>(USpacer::StaticClass(), TEXT("Spacer Item"));
			
		// Construct Hierarchy
		RootWidget->AddChild(SizeBox);
		SizeBox->AddChild(Overlay);
		
		Overlay->AddChild(BackgroundScaleBox);
		BackgroundScaleBox->AddChild(BackgroundOverlay);
		BackgroundOverlay->AddChild(DropShadow);
		BackgroundOverlay->AddChild(Background);
		BackgroundOverlay->AddChild(BackgroundOutline);
		
		Overlay->AddChild(HBox);
		HBox->AddChild(CheckScaleBox);
		CheckScaleBox->AddChild(CheckOverlay);
		CheckOverlay->AddChild(CheckBackground);
		CheckOverlay->AddChild(CheckOutline);
		CheckOverlay->AddChild(IconItem);
		HBox->AddChild(SpacerItem);
		HBox->AddChild(TextItem);

		// Additional Construction Parameters
		MinimumDimensions = FVector2D(65, 16);
		bForceCheckable = true;
		
		InitializeStyling();
		SetCheckable(true);
	}
	return Widget;
}

int32 UWidgetStudioModernCheckBox::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if (!bDisablePainting)
	{
		/* Smoothly update size */
		const float SizeX = GetDimensions().X;
		const float SizeY = GetDimensions().Y;
		UWidgetStudioFunctionLibrary::InterpSizeBoxMinOverrides(SizeBox, SizeX, SizeY, 0);
		UWidgetStudioFunctionLibrary::InterpBrushImageSize(DropShadow, SizeY, SizeY, AnimationTime);
		UWidgetStudioFunctionLibrary::InterpBrushImageSize(Background, SizeY, SizeY, AnimationTime);
		UWidgetStudioFunctionLibrary::InterpBrushImageSize(BackgroundOutline, SizeY, SizeY, AnimationTime);
		
		UWidgetStudioFunctionLibrary::InterpBrushImageSize(CheckBackground, SizeY, SizeY, AnimationTime);
		UWidgetStudioFunctionLibrary::InterpBrushImageSize(CheckOutline, SizeY, SizeY, AnimationTime);
		
		/* Smoothly update DropShadow opacity */
		const float NewDropShadowOpacity = IsHovered() ? 0.5f : 0.35f;
		UWidgetStudioFunctionLibrary::InterpWidgetOpacity(DropShadow, NewDropShadowOpacity, AnimationTime);
		
		/* Smoothly update DropShadow location */
		const float NewDropShadowLoc = IsHovered() ? 5.f : 1.f;
		UWidgetStudioFunctionLibrary::InterpWidgetTranslation(DropShadow, FVector2D(0, NewDropShadowLoc), AnimationTime);

		/* Smoothly update background color */
		UWidgetStudioFunctionLibrary::InterpImageColor(Background, BackgroundColor, AnimationTime);
		
		/* Smoothly update background opacity */
		const float NewBackgroundOpacity = Style == ECheckboxStyle::Modern ? 1.f : 0.f;
		UWidgetStudioFunctionLibrary::InterpWidgetOpacity(Background, NewBackgroundOpacity, AnimationTime);
			
		/* Smoothly update background outline color */
		UWidgetStudioFunctionLibrary::InterpImageColor(BackgroundOutline, CheckedColor, AnimationTime);

		/* Smoothly update background outline opacity */
		const float NewBackgroundOutlineOpacity = Clusivity == EClusivity::Exclusive && IsChecked() == true ? 1.0f : 0.0f;
		UWidgetStudioFunctionLibrary::InterpWidgetOpacity(BackgroundOutline, NewBackgroundOutlineOpacity, AnimationTime);
		
		/* Smoothly update check background color */
		UWidgetStudioFunctionLibrary::InterpImageColor(CheckBackground, IsChecked() ? CheckedColor : BackgroundColor, AnimationTime);

		/* Smoothly update check background opacity */
		const float NewCheckBackgroundOpacity = Style == ECheckboxStyle::Modern || IsChecked() ? 1.f : 0.f;
		UWidgetStudioFunctionLibrary::InterpWidgetOpacity(CheckBackground, NewCheckBackgroundOpacity, AnimationTime);
		
		/* Smoothly update check outline color */
		UWidgetStudioFunctionLibrary::InterpImageColor(CheckOutline, ContentColor, AnimationTime);
		
		/* Smoothly update check outline opacity */
		const float NewCheckOutlineOpacity = IsChecked() ? 0.0f : 0.35f;
		UWidgetStudioFunctionLibrary::InterpWidgetOpacity(CheckOutline, NewCheckOutlineOpacity, AnimationTime);
		
		/* Smoothly update check icon color */
		UWidgetStudioFunctionLibrary::InterpImageColor(IconItem, BackgroundColor, AnimationTime);
		
		/* Smoothly update check icon opacity */
		const float NewIconOpacity = IsChecked() ? 1.f : 0.f;
		UWidgetStudioFunctionLibrary::InterpWidgetOpacity(IconItem, NewIconOpacity, AnimationTime);
		
		/* Smoothly update text color */
		UWidgetStudioFunctionLibrary::InterpWidgetColor(TextItem, ContentColor, AnimationTime);
		
		/* Smoothly update check icon opacity */
		const float NewTextOpacity = IsChecked() ? 1.f : 0.85f;
		UWidgetStudioFunctionLibrary::InterpWidgetOpacity(TextItem, NewTextOpacity, AnimationTime);
	}
	
	return LayerId;
}

void UWidgetStudioModernCheckBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UpdateStyling();
	
	if (TextItem)
	{
		TextItem->SetText(Label);
		TextItem->SizeModifier = SizeModifier;
	}

	if (IconItem)
	{
		IconItem->SetBrushFromTexture(UWidgetStudioFunctionLibrary::GetTextureFromIconLibrary(Icon), false);
	}
}

void UWidgetStudioModernCheckBox::InitializeStyling()
{
	/* Setup Initial Styling */
	const float SizeY = GetDimensions().Y;

	// Setup Background Scale Box
	Cast<UOverlaySlot>(BackgroundScaleBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(BackgroundScaleBox->Slot)->SetVerticalAlignment(VAlign_Fill);
	Cast<UOverlaySlot>(BackgroundScaleBox->Slot)->SetPadding(0);

	// Setup Background Overlay
	Cast<UScaleBoxSlot>(BackgroundOverlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UScaleBoxSlot>(BackgroundOverlay->Slot)->SetVerticalAlignment(VAlign_Fill);

	// Setup DropShadow
	DropShadow->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedShadowMaterial());
	DropShadow->SetBrushSize(FVector2D(SizeY, SizeY));
	DropShadow->SetColorAndOpacity(FLinearColor::Black);
	DropShadow->SetRenderOpacity(0.35f);
	DropShadow->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
	DropShadow->Brush.DrawAs = ESlateBrushDrawType::Box;
	DropShadow->Brush.SetImageSize(FVector2D(SizeY, SizeY));
	DropShadow->SetVisibility(Style == ECheckboxStyle::Modern ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	Cast<UOverlaySlot>(DropShadow->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(DropShadow->Slot)->SetVerticalAlignment(VAlign_Fill);

	// Setup Background
	Background->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
	Background->SetBrushSize(FVector2D(SizeY, SizeY));
	Background->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
	Background->Brush.DrawAs = ESlateBrushDrawType::Box;
	Background->Brush.SetImageSize(FVector2D(SizeY, SizeY));
	Cast<UOverlaySlot>(Background->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(Background->Slot)->SetVerticalAlignment(VAlign_Fill);
	
	// Setup Outline
	BackgroundOutline->SetOpacity(0.f);
	BackgroundOutline->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedOutlineMaterial());
	BackgroundOutline->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
	BackgroundOutline->Brush.DrawAs = ESlateBrushDrawType::Box;
	BackgroundOutline->Brush.SetImageSize(FVector2D(SizeY, SizeY));
	BackgroundOutline->SetBrushSize(FVector2D(SizeY, SizeY));
	BackgroundOutline->SetVisibility(Style == ECheckboxStyle::Modern ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	
	Cast<UOverlaySlot>(BackgroundOutline->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(BackgroundOutline->Slot)->SetVerticalAlignment(VAlign_Fill);

	// Setup Overlay
	Cast<USizeBoxSlot>(Overlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<USizeBoxSlot>(Overlay->Slot)->SetVerticalAlignment(VAlign_Fill);
	
	// Setup HBox
	Cast<UOverlaySlot>(HBox->Slot)->SetHorizontalAlignment(HAlign_Center);
	Cast<UOverlaySlot>(HBox->Slot)->SetVerticalAlignment(VAlign_Center);
	Cast<UOverlaySlot>(HBox->Slot)->SetPadding(FVector4(15, 5, 15, 5));

	// Setup Check Scale Box
	CheckScaleBox->SetStretch(EStretch::ScaleToFitY);
	CheckScaleBox->SetVisibility(Clusivity == EClusivity::Inclusive || Style == ECheckboxStyle::Classic ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	Cast<UHorizontalBoxSlot>(CheckScaleBox->Slot)->SetPadding(2.5);
	Cast<UHorizontalBoxSlot>(CheckScaleBox->Slot)->SetHorizontalAlignment(HAlign_Center);
	Cast<UHorizontalBoxSlot>(CheckScaleBox->Slot)->SetVerticalAlignment(VAlign_Center);

	// Setup Check Background
	const float CheckBackgroundRoundness = Clusivity == EClusivity::Exclusive || Style == ECheckboxStyle::Modern ? SizeY : 5; 
	CheckBackground->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
	CheckBackground->SetBrushSize(FVector2D(CheckBackgroundRoundness, CheckBackgroundRoundness));
	CheckBackground->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
	CheckBackground->Brush.DrawAs = ESlateBrushDrawType::Box;
	CheckBackground->Brush.SetImageSize(FVector2D(CheckBackgroundRoundness, CheckBackgroundRoundness));
	Cast<UOverlaySlot>(CheckBackground->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(CheckBackground->Slot)->SetVerticalAlignment(VAlign_Fill);

	// Setup Check Outline
	const float NewCheckOutlineOpacity = IsChecked() ? 0.0f : 0.35f;
	CheckOutline->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedOutlineMaterial());
	CheckOutline->SetBrushSize(FVector2D(CheckBackgroundRoundness, CheckBackgroundRoundness));
	CheckOutline->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
	CheckOutline->Brush.DrawAs = ESlateBrushDrawType::Box;
	CheckOutline->Brush.SetImageSize(FVector2D(CheckBackgroundRoundness, CheckBackgroundRoundness));
	CheckOutline->SetOpacity(NewCheckOutlineOpacity);
	Cast<UOverlaySlot>(CheckOutline->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(CheckOutline->Slot)->SetVerticalAlignment(VAlign_Fill);

	// Setup Icon
	IconItem->SetBrushSize(FVector2D(SizeY / 2, SizeY / 2));
	IconItem->Brush.SetImageSize(FVector2D(SizeY / 2, SizeY / 2));
	Cast<UOverlaySlot>(IconItem->Slot)->SetHorizontalAlignment(HAlign_Center);
	Cast<UOverlaySlot>(IconItem->Slot)->SetVerticalAlignment(VAlign_Center);
	Cast<UOverlaySlot>(IconItem->Slot)->SetPadding(Clusivity == EClusivity::Inclusive ? 2.5 : 0);

	// Setup Spacer
	SpacerItem->SetSize(FVector2D(15, 0));
	SpacerItem->SetVisibility(Clusivity == EClusivity::Inclusive || Style == ECheckboxStyle::Classic ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	// Setup Text
	TextItem->bDisablePainting = true;
	TextItem->SetColor(EPalette::White);
	TextItem->SetType(EFontType::Caption);
	TextItem->SetJustification(Clusivity == EClusivity::Inclusive ? ETextJustify::Left : ETextJustify::Center);
	Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetHorizontalAlignment(HAlign_Center);
	Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetVerticalAlignment(VAlign_Center);

	// Setup Colors
	Background->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(BackgroundColor));
	BackgroundOutline->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(CheckedColor));
	TextItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(ContentColor));
	IconItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(BackgroundColor));
	CheckBackground->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(IsChecked() ? CheckedColor : BackgroundColor));
	CheckOutline->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(ContentColor));	
}

void UWidgetStudioModernCheckBox::UpdateStyling()
{
	// Update Size Box
	SizeBox->SetMinDesiredWidth(GetDimensions().Y * 2.5);
	SizeBox->SetHeightOverride(GetDimensions().Y);
}

FText UWidgetStudioModernCheckBox::GetLabel() const
{
	return Label;
}

void UWidgetStudioModernCheckBox::SetLabel(const FText NewLabel)
{
	Label = NewLabel;
	SynchronizeProperties();
}

EIconItem UWidgetStudioModernCheckBox::GetIcon() const
{
	return Icon;
}

EPalette UWidgetStudioModernCheckBox::GetBackgroundColor() const
{
	return BackgroundColor;
}

EPalette UWidgetStudioModernCheckBox::GetContentColor() const
{
	return ContentColor;
}

EPalette UWidgetStudioModernCheckBox::GetCheckedColor() const
{
	return CheckedColor;
}

EClusivity UWidgetStudioModernCheckBox::GetSelectionMethod() const
{
	return Clusivity;
}

void UWidgetStudioModernCheckBox::SetSelectionMethod(const EClusivity NewState)
{
	Clusivity = NewState;
}

void UWidgetStudioModernCheckBox::SetIcon(const EIconItem NewIcon)
{
	Icon = NewIcon;
	SynchronizeProperties();
}

void UWidgetStudioModernCheckBox::SetBackgroundColor(const EPalette NewColor)
{
	BackgroundColor = NewColor;
}

void UWidgetStudioModernCheckBox::SetContentColor(const EPalette NewColor)
{
	ContentColor = NewColor;
}

void UWidgetStudioModernCheckBox::SetCheckedColor(const EPalette NewColor)
{
	CheckedColor = NewColor;
}


