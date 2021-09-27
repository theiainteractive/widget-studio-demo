// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WSModernComboBox.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/OverlaySlot.h"
#include "Components/ScaleBoxSlot.h"
#include "Components/SizeBoxSlot.h"
#include "Components/VerticalBoxSlot.h"
#include "Widgets/WSModernTabBar.h"

#define LOCTEXT_NAMESPACE "WidgetStudio"

#if WITH_EDITOR
const FText UWidgetStudioModernComboBox::GetPaletteCategory()
{
	return LOCTEXT("WidgetStudio", "*Widget Studio - Modern");
}
#endif

#undef LOCTEXT_NAMESPACE

TSharedRef<SWidget> UWidgetStudioModernComboBox::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;
	
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{

		/*
		* RootWidget (Overlay)
		* - VBox
		* -- Label Item
		* -- Content Size Box
		* --- Content Scale Box
		* ---- Content Overlay
		* ----- DropShadow
		* ----- Background
		* ---- HBox
		* ----- Icon Item
		* ----- Text Item
		* ----- Arrow Item
		* --- Menu Anchor
		*/
		
		// Construct Widgets 
		VBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("VBox"));
		LabelItem = WidgetTree->ConstructWidget<UWidgetStudioText>(UWidgetStudioText::StaticClass(), TEXT("Label Item"));
		ContentSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Content Size Box"));
		ContentScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Content Scale Box"));
		ContentOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Content Overlay"));
		DropShadow = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("DropShadow"));
		Background = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Background"));
		HBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("HBox"));
		IconItem = WidgetTree->ConstructWidget<UWidgetStudioIcon>(UWidgetStudioIcon::StaticClass(), TEXT("Icon Item"));
		TextItem = WidgetTree->ConstructWidget<UWidgetStudioText>(UWidgetStudioText::StaticClass(), TEXT("Text Item"));
		ArrowItem = WidgetTree->ConstructWidget<UWidgetStudioIcon>(UWidgetStudioIcon::StaticClass(), TEXT("Arrow Item"));
		MenuAnchor = WidgetTree->ConstructWidget<UMenuAnchor>(UMenuAnchor::StaticClass(), TEXT("Menu Anchor"));

				
		// Construct Hierarchy 	
		RootWidget->AddChild(VBox);

		VBox->AddChild(LabelItem);
		VBox->AddChild(ContentSizeBox);

		ContentSizeBox->AddChild(ContentScaleBox);
		ContentScaleBox->AddChild(ContentOverlay);
		ContentOverlay->AddChild(DropShadow);
		ContentOverlay->AddChild(Background);
		ContentOverlay->AddChild(HBox);
		ContentOverlay->AddChild(MenuAnchor);

		HBox->AddChild(IconItem);
		HBox->AddChild(TextItem);
		HBox->AddChild(ArrowItem);
		
		// Additional Construction Parameters
		MinimumDimensions = FVector2D(225, 35);

		InitializeStyling();
		
	}
	return Widget;
}

int32 UWidgetStudioModernComboBox::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if (!bDisablePainting)
	{
		// Smoothly Update SizeBox 
		UWidgetStudioFunctionLibrary::InterpSizeBoxMinOverrides(ContentSizeBox, GetDimensions().X, 0, AnimationTime);
		UWidgetStudioFunctionLibrary::InterpSizeBoxOverrides(ContentSizeBox, 0, GetDimensions().Y, AnimationTime);
		
		// Smoothly update DropShadow opacity 
		const float NewDropShadowOpacity = MenuAnchor->IsOpen() || IsHovered() ? 0.5f : 0.35f;
		UWidgetStudioFunctionLibrary::InterpWidgetOpacity(DropShadow, NewDropShadowOpacity, AnimationTime);
		
		// Smoothly update DropShadow location 
		const float NewDropShadowLoc = MenuAnchor->IsOpen() || IsHovered() ? 5.f : 1.f;
		UWidgetStudioFunctionLibrary::InterpWidgetTranslation(DropShadow, FVector2D(0, NewDropShadowLoc), AnimationTime);
		
		UWidgetStudioFunctionLibrary::InterpImageColor(Background, BackgroundColor, AnimationTime);
		UWidgetStudioFunctionLibrary::InterpWidgetColor(IconItem, ContentColor, AnimationTime);
		UWidgetStudioFunctionLibrary::InterpWidgetColor(ArrowItem, ContentColor, AnimationTime);
		UWidgetStudioFunctionLibrary::InterpWidgetRotation(ArrowItem, MenuAnchor->IsOpen() ? 180 : 0, AnimationTime * 4);
		UWidgetStudioFunctionLibrary::InterpWidgetColor(LabelItem, LabelColor, AnimationTime);
		UWidgetStudioFunctionLibrary::InterpWidgetColor(TextItem, ContentColor, AnimationTime);
		UWidgetStudioFunctionLibrary::InterpWidgetOpacity(TextItem, GetCurrentIndex() != -1 || bIsSelectable ? 1.0f : 0.5f, AnimationTime);
	}

	return LayerId;
}

void UWidgetStudioModernComboBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UpdateStyling();
}

void UWidgetStudioModernComboBox::InitializeStyling()
{
	Super::InitializeStyling();
	
	const float SizeY = GetDimensions().Y;
	const float RoundedSize = CornerStyle == ECornerStyle::Pill ? SizeY : CornerStyle == ECornerStyle::Rounded ? 15 : 0;

	// Setup VBox
	Cast<UOverlaySlot>(VBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(VBox->Slot)->SetVerticalAlignment(VAlign_Fill);

	// Setup Label
	LabelItem->bDisablePainting = true;
	LabelItem->SetColor(EPalette::White);
	LabelItem->SetType(EFontType::Caption);
	Cast<UVerticalBoxSlot>(LabelItem->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UVerticalBoxSlot>(LabelItem->Slot)->SetVerticalAlignment(VAlign_Fill);
	Cast<UVerticalBoxSlot>(LabelItem->Slot)->SetPadding(FMargin(0, 0, 0, 3));

	// Setup Content Size Box
	ContentSizeBox->SetMinDesiredWidth(GetDimensions().X);
	ContentSizeBox->SetHeightOverride(SizeY);
	
	Cast<UVerticalBoxSlot>(ContentSizeBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UVerticalBoxSlot>(ContentSizeBox->Slot)->SetVerticalAlignment(VAlign_Fill);
	//Cast<UVerticalBoxSlot>(ContentSizeBox->Slot)->SetSize(ESlateSizeRule::Fill);

	// Content Scale Box
	ContentScaleBox->SetStretch(CornerStyle == ECornerStyle::Rounded || CornerStyle == ECornerStyle::Pill ? EStretch::Fill : EStretch::ScaleToFill);
	Cast<USizeBoxSlot>(ContentScaleBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<USizeBoxSlot>(ContentScaleBox->Slot)->SetVerticalAlignment(VAlign_Fill);

	// Setup Content Overlay
	Cast<UScaleBoxSlot>(ContentOverlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UScaleBoxSlot>(ContentOverlay->Slot)->SetVerticalAlignment(VAlign_Fill);

	// Setup DropShadow
	DropShadow->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedShadowMaterial());
	DropShadow->SetBrushSize(FVector2D(RoundedSize, RoundedSize));
	DropShadow->SetColorAndOpacity(FLinearColor::Black);
	DropShadow->SetRenderOpacity(0.35f);
	DropShadow->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
	DropShadow->Brush.DrawAs = ESlateBrushDrawType::Box;
	DropShadow->Brush.SetImageSize(FVector2D(RoundedSize, RoundedSize));
	Cast<UOverlaySlot>(DropShadow->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(DropShadow->Slot)->SetVerticalAlignment(VAlign_Fill);

	// Setup Background
	Background->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
	Background->SetBrushSize(FVector2D(RoundedSize, RoundedSize));
	Background->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(BackgroundColor));
	Background->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
	Background->Brush.DrawAs = ESlateBrushDrawType::Box;
	Background->Brush.SetImageSize(FVector2D(RoundedSize, RoundedSize));
	Cast<UOverlaySlot>(Background->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(Background->Slot)->SetVerticalAlignment(VAlign_Fill);

	// Setup HBox
	Cast<UOverlaySlot>(HBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(HBox->Slot)->SetVerticalAlignment(VAlign_Center);
	Cast<UOverlaySlot>(HBox->Slot)->SetPadding(FMargin(15, 0, 15, 0));
	
	// Setup Icon
	IconItem->bDisablePainting = true;
	IconItem->SetColor(EPalette::White);
	Cast<UHorizontalBoxSlot>(IconItem->Slot)->SetHorizontalAlignment(HAlign_Left);
	Cast<UHorizontalBoxSlot>(IconItem->Slot)->SetVerticalAlignment(VAlign_Center);
	Cast<UHorizontalBoxSlot>(IconItem->Slot)->SetPadding(FMargin(0, 0, 15, 0));

	// Setup Arrow
	ArrowItem->bDisablePainting = true;
	ArrowItem->SetColor(EPalette::White);
	ArrowItem->SetIcon(EIconItem::ChevronDown);
	Cast<UHorizontalBoxSlot>(ArrowItem->Slot)->SetHorizontalAlignment(HAlign_Right);
	Cast<UHorizontalBoxSlot>(ArrowItem->Slot)->SetVerticalAlignment(VAlign_Center);

	// Setup Text
	TextItem->bDisablePainting = true;
	TextItem->SetColor(EPalette::White);
	TextItem->SetType(EFontType::Body2);
	Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetVerticalAlignment(VAlign_Center);
	Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetSize(ESlateSizeRule::Fill);
	Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetPadding(FMargin(0, 0, 15, 0));

	// Setup Menu Anchor
	MenuAnchor->SetPlacement(MenuPlacement_ComboBox);
	MenuAnchor->FitInWindow(true);
	Cast<UOverlaySlot>(MenuAnchor->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(MenuAnchor->Slot)->SetVerticalAlignment(VAlign_Bottom);

	// Setup Colors
	Background->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(BackgroundColor));
	IconItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(ContentColor));
	ArrowItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(ContentColor));
	LabelItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(LabelColor));
	TextItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(ContentColor));

	// Setup Bindings
	MenuAnchor->OnGetUserMenuContentEvent.BindUFunction(this, "ConstructMenu");
}

void UWidgetStudioModernComboBox::UpdateStyling()
{
	Super::UpdateStyling();

	if (ContentSizeBox)
	{
		ContentSizeBox->SetMinDesiredWidth(GetDimensions().X);
		ContentSizeBox->SetHeightOverride(GetDimensions().Y);
	}
	

	if (LabelItem)
	{
		LabelItem->SetText(Label);
		LabelItem->SetVisibility(Label.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

		LabelItem->SizeModifier = SizeModifier;
	}
	
	if (TextItem)
	{
		TextItem->SizeModifier = SizeModifier;
		
		if (GetCurrentIndex() > -1 && bIsSelectable)
		{
			const auto Option = GetCurrentOption();
			TextItem->SetText(Option.Text);
			TextItem->SetVisibility(Option.ContentStyle == EButtonContentStyle::Label || Option.ContentStyle == EButtonContentStyle::IconWithLabel ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

			IconItem->SetCustomIcon(UWidgetStudioFunctionLibrary::GetTextureFromIconLibrary(Option.Icon));
			IconItem->SetVisibility(Option.ContentStyle == EButtonContentStyle::Icon || Option.ContentStyle == EButtonContentStyle::IconWithLabel ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
		else
		{
			TextItem->SetVisibility(ESlateVisibility::HitTestInvisible);
			TextItem->SetText(PlaceholderText);

			IconItem->SetVisibility(ESlateVisibility::Collapsed);
			IconItem->SetCustomIcon(nullptr);
		}
	}

	if (IconItem)
	{
		IconItem->SizeModifier = SizeModifier;
		IconItem->SetSize(GetDimensions().Y / 2);
	}

	if (ArrowItem)
	{
		ArrowItem->SizeModifier = SizeModifier;
		ArrowItem->SetSize(GetDimensions().Y / 3);
	}
}

void UWidgetStudioModernComboBox::SetCurrentIndex(const int32 Index)
{
	Super::SetCurrentIndex(Index);
	SynchronizeProperties();
	UpdateStyling();
}

FReply UWidgetStudioModernComboBox::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(FKey("LeftMouseButton")))
	{
		MenuAnchor->Open(true);

		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UWidgetStudioModernComboBox::OnMenuButtonSelected(const int32 Index, FButtonOptions Option)
{
	SetCurrentIndex(Index);
	SynchronizeProperties();
	MenuAnchor->Close();
}

UUserWidget* UWidgetStudioModernComboBox::ConstructMenu() const
{
	UWidgetStudioModernTabBar* MenuBar = CreateWidget<UWidgetStudioModernTabBar>(MenuAnchor);
	MenuBar->SetScrollable(true);
	MenuBar->SetSizeConstraint(MenuHeight);
	MenuBar->SizeModifier = SizeModifier;
	MenuBar->OverrideDimensions = OverrideDimensions;
	MenuBar->SetOrientation(Orient_Vertical);
	MenuBar->SetSizeConstraint(MenuMaxHeight);
	MenuBar->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	MenuBar->SetSelectable(bIsSelectable);
	MenuBar->SetBackgroundColor(BackgroundColor);
	MenuBar->SetContentColor(ContentColor);
	MenuBar->SetSelectionColor(SelectionColor);
	MenuBar->SetOptions(GetOptions());
	MenuBar->ForceStyleUpdate();
	
	MenuBar->OnCurrentIndexChanged.AddDynamic(this, &UWidgetStudioModernComboBox::OnMenuButtonSelected);

	return MenuBar;
}

FText UWidgetStudioModernComboBox::GetLabel() const
{
	return Label;
}

FText UWidgetStudioModernComboBox::GetPlaceholderText() const
{
	return PlaceholderText;
}

ECornerStyle UWidgetStudioModernComboBox::GetCornerStyle() const
{
	return CornerStyle;
}

EPalette UWidgetStudioModernComboBox::GetBackgroundColor() const
{
	return BackgroundColor;
}

EPalette UWidgetStudioModernComboBox::GetContentColor() const
{
	return ContentColor;
}

EPalette UWidgetStudioModernComboBox::GetSelectionColor() const
{
	return SelectionColor;
}

EPalette UWidgetStudioModernComboBox::GetLabelColor() const
{
	return LabelColor;
}

bool UWidgetStudioModernComboBox::IsSelectable() const
{
	return bIsSelectable;
}

void UWidgetStudioModernComboBox::SetLabel(const FText NewLabel)
{
	Label = NewLabel;
	SynchronizeProperties();
}

void UWidgetStudioModernComboBox::SetPlaceholderText(const FText NewText)
{
	PlaceholderText = NewText;
}

void UWidgetStudioModernComboBox::SetCornerStyle(const ECornerStyle NewStyle)
{
	CornerStyle = NewStyle;
}

void UWidgetStudioModernComboBox::SetBackgroundColor(const EPalette NewColor)
{
	BackgroundColor = NewColor;
}

void UWidgetStudioModernComboBox::SetContentColor(const EPalette NewColor)
{
	ContentColor = NewColor;
}

void UWidgetStudioModernComboBox::SetSelectionColor(const EPalette NewColor)
{
	SelectionColor = NewColor;
}

void UWidgetStudioModernComboBox::SetLabelColor(const EPalette NewColor)
{
	LabelColor = NewColor;
}

void UWidgetStudioModernComboBox::SetSelectable(const bool NewState)
{
	bIsSelectable = NewState;
}
