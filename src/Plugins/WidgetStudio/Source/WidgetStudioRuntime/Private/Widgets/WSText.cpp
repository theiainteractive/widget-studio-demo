// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WSText.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

#define LOCTEXT_NAMESPACE "WidgetStudio"

#if WITH_EDITOR
const FText UWidgetStudioText::GetPaletteCategory()
{
	return LOCTEXT("WidgetStudio", "*Widget Studio");
}
#endif

#undef LOCTEXT_NAMESPACE

TSharedRef<SWidget> UWidgetStudioText::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;

	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{
		/* Construct Widgets */
		TextItem = WidgetTree->ConstructWidget<UWidgetStudioTextBlock>(UWidgetStudioTextBlock::StaticClass(), TEXT("TextItem"));
			
		/* Construct Hierarchy */
		RootWidget->AddChild(TextItem);

		// Additional Construction Parameters
		bCanOverrideDimensions = false;

		InitializeStyling();
		
	}
	return Widget;
}

int32 UWidgetStudioText::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	if (!bDisablePainting)
	{
		// Adjusts icon color
		UWidgetStudioFunctionLibrary::InterpTextColor(TextItem, Color, AnimationTime);
	}
	return LayerId;
}

void UWidgetStudioText::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UpdateStyling();
}

void UWidgetStudioText::InitializeStyling()
{
	Super::InitializeStyling();

	// Setup Colors
	TextItem->SetColorAndOpacity(FSlateColor(UWidgetStudioFunctionLibrary::GetColorFromPalette(Color)));
	Cast<UOverlaySlot>(TextItem->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(TextItem->Slot)->SetVerticalAlignment(VAlign_Fill);
}

void UWidgetStudioText::UpdateStyling()
{
	Super::UpdateStyling();

	if (TextItem)
	{
		FontInfo = Type == EFontType::Custom ? UWidgetStudioFunctionLibrary::ConstructFontInfoFromStyle(CustomStyle) : UWidgetStudioFunctionLibrary::ConstructFontInfoFromTypography(Type);
		FontInfo.Size = UWidgetStudioFunctionLibrary::GetSizeByModifier(SizeModifier, FontInfo.Size);
		TextItem->SetFont(FontInfo);
		TextItem->SetText(Case == EFontCase::Uppercase ? Text.ToUpper() : Case == EFontCase::Lowercase ? Text.ToLower() : Text);
		TextItem->SetJustification(Justification);
		TextItem->SetAutoWrapText(AutoWrapText);
		
		if (bDisablePainting)
		{
			TextItem->SetColorAndOpacity(FSlateColor(UWidgetStudioFunctionLibrary::GetColorFromPalette(Color)));
		}
	}
}

FText UWidgetStudioText::GetText() const
{
	return Text;
}

bool UWidgetStudioText::IsTextEmpty() const
{
	return Text.IsEmpty();
}

ETextJustify::Type UWidgetStudioText::GetJustification() const
{
	return Justification;
}

int32 UWidgetStudioText::Length() const
{
	return Text.ToString().Len();
}

EFontType UWidgetStudioText::GetType() const
{
	return Type;
}

FFontStyle UWidgetStudioText::GetCustomStyle() const
{
	return CustomStyle;
}

void UWidgetStudioText::SetCustomStyle(const FFontStyle NewStyle)
{
	CustomStyle = NewStyle;
	SynchronizeProperties();
}

bool UWidgetStudioText::IsAutoWrappingText() const
{
	return AutoWrapText;
}

float UWidgetStudioText::GetWrapTextAt() const
{
	return WrapTextAt;
}

ETextWrappingPolicy UWidgetStudioText::GetWrapPolicy() const
{
	return WrappingPolicy;
}

EFontCase UWidgetStudioText::GetCase() const
{
	return Case;
}

void UWidgetStudioText::SetCase(const EFontCase NewCase)
{
	Case = NewCase;
	SynchronizeProperties();
}

void UWidgetStudioText::SetText(const FText NewText)
{
	Text = NewText;
	SynchronizeProperties();
}

void UWidgetStudioText::SetJustification(const ETextJustify::Type NewJustification)
{
	Justification = NewJustification;
	SynchronizeProperties();
}

void UWidgetStudioText::SetColor(const EPalette NewColor)
{
	Color = NewColor;
	SynchronizeProperties();
}

void UWidgetStudioText::SetType(const EFontType NewType)
{
	Type = NewType;
	SynchronizeProperties();
}

void UWidgetStudioText::SetAutoWrapText(const bool NewState)
{
	AutoWrapText = NewState;
	TextItem->SetAutoWrapText(NewState);
	SynchronizeProperties();
}

void UWidgetStudioText::SetWrapTextAt(const float NewWrapAt)
{
	WrapTextAt = NewWrapAt;
	TextItem->SetWrapTextAt(NewWrapAt);
	SynchronizeProperties();
}

void UWidgetStudioText::SetWrapPolicy(const ETextWrappingPolicy NewPolicy)
{
	WrappingPolicy = NewPolicy;
	TextItem->SetWrappingPolicy(NewPolicy);
	SynchronizeProperties();
}
