// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WSModernTextField.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/OverlaySlot.h"
#include "Components/ScaleBoxSlot.h"
#include "Components/SizeBoxSlot.h"
#include "Components/VerticalBoxSlot.h"
#include "Engine/Font.h"

#define LOCTEXT_NAMESPACE "WidgetStudio"

#if WITH_EDITOR
const FText UWidgetStudioModernTextField::GetPaletteCategory()
{
	return LOCTEXT("WidgetStudio", "*Widget Studio - Modern");
}
#endif

#undef LOCTEXT_NAMESPACE

TSharedRef<SWidget> UWidgetStudioModernTextField::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;
	
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{

		/*
		 * RootWidget (RetainerBox)
		 * - VBox
		 * -- Label Item
		 * -- Size Box
		 * --- Overlay
		 * ---- Background Scale Box
		 * ----- Background Overlay
		 * ------ DropShadow
		 * ------ Background
		 * ------ Background Outline
		 * ---- HBox
		 * ----- Leading Icon Scale Box
		 * ------ Leading Icon
		 * ----- Editable Text Box
		 * ----- Trailing Icon Scale Box
		 * ------ Trailing Icon
		 */
		
		/* Construct Widgets */
		Overlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Overlay"));
		VBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("VBox"));
		LabelItem = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Label Item"));
		SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("SizeBox"));
		BackgroundScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Background Scale Box"));
		BackgroundOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Background Overlay"));
		DropShadow = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("DropDown"));
		Background = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Background"));
		BackgroundOutline = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Background Outline"));
		HBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("HBox"));
		
		LeadingIconScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Leading Icon Scale Box"));
		LeadingIconItem = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Leading Icon"));
		TrailingIconScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Trailing Icon Scale Box"));
		TrailingIconItem = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Trailing Icon"));

		TextInput = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("Text Input"));
			
		/* Construct Hierarchy */
		RootWidget->AddChild(VBox);
		VBox->AddChild(LabelItem);
		VBox->AddChild(SizeBox);

		SizeBox->AddChild(Overlay);
		Overlay->AddChild(BackgroundScaleBox);
		Overlay->AddChild(HBox);

		BackgroundScaleBox->AddChild(BackgroundOverlay);
		BackgroundOverlay->AddChild(DropShadow);
		BackgroundOverlay->AddChild(Background);
		BackgroundOverlay->AddChild(BackgroundOutline);

		HBox->AddChild(LeadingIconScaleBox);
		HBox->AddChild(TextInput);
		HBox->AddChild(TrailingIconScaleBox);

		LeadingIconScaleBox->AddChild(LeadingIconItem);
		TrailingIconScaleBox->AddChild(TrailingIconItem);

		InitializeStyling();
	}
	
	return Widget;
}

int32 UWidgetStudioModernTextField::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if (!bDisablePainting)
	{
		/* Smoothly update size box */
		UWidgetStudioFunctionLibrary::InterpSizeBoxMinOverrides(SizeBox, GetDimensions().X, 0, AnimationTime);
		
		/* Smoothly update DropShadow opacity */
		const float NewDropShadowOpacity = TextInput->HasKeyboardFocus() || IsHovered() ? 0.5f : 0.35f;
		
		UWidgetStudioFunctionLibrary::InterpWidgetOpacity(DropShadow, NewDropShadowOpacity, AnimationTime);
		
		/* Smoothly update DropShadow location */
		const float NewDropShadowLoc = TextInput->HasKeyboardFocus() || IsHovered() ? 5.f : 1.f;
		UWidgetStudioFunctionLibrary::InterpWidgetTranslation(DropShadow, FVector2D(0, NewDropShadowLoc), AnimationTime);
		
		UWidgetStudioFunctionLibrary::InterpImageColor(Background, BackgroundColor, AnimationTime);
		UWidgetStudioFunctionLibrary::InterpImageColor(BackgroundOutline, GetColorByState(), AnimationTime);
		UWidgetStudioFunctionLibrary::InterpImageColor(LeadingIconItem, ContentColor, AnimationTime);
		UWidgetStudioFunctionLibrary::InterpImageColor(TrailingIconItem, ContentColor, AnimationTime);
		UWidgetStudioFunctionLibrary::InterpEditableTextBoxColor(TextInput, ContentColor, AnimationTime);
		UWidgetStudioFunctionLibrary::InterpTextColor(LabelItem, LabelColor, AnimationTime);
	}

	return LayerId;
}

void UWidgetStudioModernTextField::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UpdateStyling();
		
	if (TextInput)
	{
		TextInput->SetIsPassword(bIsPasswordField);
		TextInput->SetHintText(PlaceholderText);
		TextInput->SetText(Text);
	}

	if (LabelItem)
	{
		LabelItem->SetText(Label);
		LabelItem->SetVisibility(Label.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}

	if (LeadingIconItem)
	{
		LeadingIconItem->SetBrushFromTexture(UWidgetStudioFunctionLibrary::GetTextureFromIconLibrary(LeadingIcon), false);
		LeadingIconItem->SetBrushSize(FVector2D(GetIconSize(), GetIconSize()));
		LeadingIconItem->Brush.SetImageSize(FVector2D(GetIconSize(), GetIconSize()));
	}

	if (TrailingIconItem)
	{
		TrailingIconItem->SetBrushFromTexture(UWidgetStudioFunctionLibrary::GetTextureFromIconLibrary(TrailingIcon), false);
		TrailingIconItem->SetBrushSize(FVector2D(GetIconSize(), GetIconSize()));
		TrailingIconItem->Brush.SetImageSize(FVector2D(GetIconSize(), GetIconSize()));
	}

	if (LeadingIconItem && TrailingIconItem)
	{
		LeadingIconItem->SetBrushFromTexture(UWidgetStudioFunctionLibrary::GetTextureFromIconLibrary(LeadingIcon), false);
		TrailingIconItem->SetBrushFromTexture(UWidgetStudioFunctionLibrary::GetTextureFromIconLibrary(TrailingIcon), false);
		if (IconVisibility == ETextFieldIconVisibility::LeadingAndTrailing)
		{
			LeadingIconScaleBox->SetVisibility(ESlateVisibility::HitTestInvisible);
			TrailingIconScaleBox->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (IconVisibility == ETextFieldIconVisibility::Leading)
		{
			LeadingIconScaleBox->SetVisibility(ESlateVisibility::HitTestInvisible);
			TrailingIconScaleBox->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IconVisibility == ETextFieldIconVisibility::Trailing)
		{
			LeadingIconScaleBox->SetVisibility(ESlateVisibility::Collapsed);
			TrailingIconScaleBox->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	
		if (IconVisibility == ETextFieldIconVisibility::Hidden)
		{
			LeadingIconScaleBox->SetVisibility(ESlateVisibility::Collapsed);
			TrailingIconScaleBox->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UWidgetStudioModernTextField::InitializeStyling()
{
	Super::InitializeStyling();
	
	const float SizeY = GetDimensions().Y;
	const float RoundedSize = CornerStyle == ECornerStyle::Pill ?  SizeY : 15;

	// Setup VBox
	Cast<UOverlaySlot>(VBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(VBox->Slot)->SetVerticalAlignment(VAlign_Fill);

	// Setup Label
	LabelItem->SetFont(UWidgetStudioFunctionLibrary::ConstructFontInfoFromTypography(EFontType::Caption));
	Cast<UVerticalBoxSlot>(LabelItem->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UVerticalBoxSlot>(LabelItem->Slot)->SetVerticalAlignment(VAlign_Fill);
	Cast<UVerticalBoxSlot>(LabelItem->Slot)->SetPadding(FMargin(0, 0, 0, 3));

	// Setup Size Box
	SizeBox->SetHeightOverride(SizeY);
	SizeBox->SetMinDesiredWidth(GetDimensions().X);
	Cast<UVerticalBoxSlot>(SizeBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UVerticalBoxSlot>(SizeBox->Slot)->SetVerticalAlignment(VAlign_Fill);

	// Setup Overlay
	Cast<USizeBoxSlot>(Overlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<USizeBoxSlot>(Overlay->Slot)->SetVerticalAlignment(VAlign_Fill);

	/* Setup Background Scale Box */
	BackgroundScaleBox->SetStretch(CornerStyle == ECornerStyle::Rounded || CornerStyle == ECornerStyle::Pill ? EStretch::Fill : EStretch::ScaleToFill);
	Cast<UOverlaySlot>(BackgroundScaleBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(BackgroundScaleBox->Slot)->SetVerticalAlignment(VAlign_Fill);
	Cast<UOverlaySlot>(BackgroundScaleBox->Slot)->SetPadding(0);

	/* Setup Background Overlay */
	Cast<UScaleBoxSlot>(BackgroundOverlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UScaleBoxSlot>(BackgroundOverlay->Slot)->SetVerticalAlignment(VAlign_Fill);

	/* Setup DropShadow */
	DropShadow->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedShadowMaterial());
	DropShadow->SetBrushSize(FVector2D(RoundedSize, RoundedSize));
	DropShadow->SetColorAndOpacity(FLinearColor::Black);
	DropShadow->SetRenderOpacity(0.35f);
	DropShadow->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
	DropShadow->Brush.DrawAs = ESlateBrushDrawType::Box;
	DropShadow->Brush.SetImageSize(FVector2D(RoundedSize, RoundedSize));
	Cast<UOverlaySlot>(DropShadow->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(DropShadow->Slot)->SetVerticalAlignment(VAlign_Fill);

	/* Setup Background */
	Background->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
	Background->SetBrushSize(FVector2D(RoundedSize, RoundedSize));
	Background->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
	Background->Brush.DrawAs = ESlateBrushDrawType::Box;
	Background->Brush.SetImageSize(FVector2D(RoundedSize, RoundedSize));
	Cast<UOverlaySlot>(Background->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(Background->Slot)->SetVerticalAlignment(VAlign_Fill);

	/* Setup Outline */
	BackgroundOutline->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedOutlineMaterial());
	BackgroundOutline->SetBrushSize(FVector2D(RoundedSize, RoundedSize));
	BackgroundOutline->Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
	BackgroundOutline->Brush.DrawAs = ESlateBrushDrawType::Box;
	BackgroundOutline->Brush.SetImageSize(FVector2D(RoundedSize, RoundedSize));
	Cast<UOverlaySlot>(BackgroundOutline->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(BackgroundOutline->Slot)->SetVerticalAlignment(VAlign_Fill);

	// Setup Horizontal Box
	Cast<UOverlaySlot>(HBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(HBox->Slot)->SetVerticalAlignment(VAlign_Center);
	Cast<UOverlaySlot>(HBox->Slot)->SetPadding(FMargin(15, 0, 15, 0));

	// Setup Leading Icon Scale Box
	LeadingIconScaleBox->SetStretch(EStretch::ScaleToFitY);
	Cast<UHorizontalBoxSlot>(LeadingIconScaleBox->Slot)->SetHorizontalAlignment(HAlign_Left);
	Cast<UHorizontalBoxSlot>(LeadingIconScaleBox->Slot)->SetVerticalAlignment(VAlign_Center);
	Cast<UHorizontalBoxSlot>(LeadingIconScaleBox->Slot)->SetPadding(FMargin(0, 2.5, 15, 2.5));

	/* Setup Leading Icon */
	Cast<UScaleBoxSlot>(LeadingIconItem->Slot)->SetHorizontalAlignment(HAlign_Center);
	Cast<UScaleBoxSlot>(LeadingIconItem->Slot)->SetVerticalAlignment(VAlign_Center);

	// Setup Trailing Icon Scale Box
	TrailingIconScaleBox->SetStretch(EStretch::ScaleToFitY);
	Cast<UHorizontalBoxSlot>(TrailingIconScaleBox->Slot)->SetHorizontalAlignment(HAlign_Right);
	Cast<UHorizontalBoxSlot>(TrailingIconScaleBox->Slot)->SetVerticalAlignment(VAlign_Center);
	Cast<UHorizontalBoxSlot>(TrailingIconScaleBox->Slot)->SetPadding(FMargin(15, 2.5, 0, 2.5));

	/* Setup Trailing Icon */
	Cast<UScaleBoxSlot>(TrailingIconItem->Slot)->SetHorizontalAlignment(HAlign_Center);
	Cast<UScaleBoxSlot>(TrailingIconItem->Slot)->SetVerticalAlignment(VAlign_Center);

	// Setup Text Box
	TextInput->WidgetStyle.SetFont(UWidgetStudioFunctionLibrary::ConstructFontInfoFromTypography(EFontType::Body2));
	TextInput->WidgetStyle.BackgroundImageNormal.DrawAs = ESlateBrushDrawType::NoDrawType;
	TextInput->WidgetStyle.BackgroundImageHovered.DrawAs = ESlateBrushDrawType::NoDrawType;
	TextInput->WidgetStyle.BackgroundImageFocused.DrawAs = ESlateBrushDrawType::NoDrawType;
	TextInput->WidgetStyle.BackgroundImageReadOnly.DrawAs = ESlateBrushDrawType::NoDrawType;
	Cast<UHorizontalBoxSlot>(TextInput->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UHorizontalBoxSlot>(TextInput->Slot)->SetVerticalAlignment(VAlign_Center);
	Cast<UHorizontalBoxSlot>(TextInput->Slot)->SetSize(ESlateSizeRule::Fill);

	TextInput->OnTextChanged.RemoveAll(this);
	TextInput->OnTextCommitted.RemoveAll(this);
	TextInput->OnTextChanged.AddDynamic(this, &UWidgetStudioModernTextField::OnTextInputTextChanged);
	TextInput->OnTextCommitted.AddDynamic(this, &UWidgetStudioModernTextField::OnTextInputTextCommitted);

	// Setup Colors
	Background->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(BackgroundColor));
	BackgroundOutline->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(GetColorByState()));
	LeadingIconItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(ContentColor));
	TrailingIconItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(ContentColor));
	TextInput->WidgetStyle.SetForegroundColor(UWidgetStudioFunctionLibrary::GetColorFromPalette(ContentColor));
	LabelItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(LabelColor));
}

void UWidgetStudioModernTextField::UpdateStyling()
{
	Super::UpdateStyling();
	
	SizeBox->SetMinDesiredWidth(GetDimensions().X);
	SizeBox->SetHeightOverride(GetDimensions().Y);
}

void UWidgetStudioModernTextField::OnTextInputTextChanged(const FText& InText)
{
	SetText(InText);
}

void UWidgetStudioModernTextField::OnTextInputTextCommitted(const FText& InText, const ETextCommit::Type InCommitMethod)
{
	SetText(InText);
	OnTextCommitted.Broadcast(Text, InCommitMethod);
}

EPalette UWidgetStudioModernTextField::GetColorByState() const
{
	if (State == ETextFieldState::Normal) { return DefaultOutline; }
	if (State == ETextFieldState::Accepted) { return EPalette::Green; }
	if (State == ETextFieldState::Error) { return EPalette::Red; }
	if (State == ETextFieldState::Warning) { return EPalette::Yellow; }
	if (State == ETextFieldState::Focused) { return ContentColor; }

	return EPalette::Transparent;
}

FText UWidgetStudioModernTextField::GetLabel() const
{
	return Label;
}

FText UWidgetStudioModernTextField::GetText() const
{
	return Text;
}

FText UWidgetStudioModernTextField::GetPlaceholderText() const
{
	return PlaceholderText;
}

ETextFilter UWidgetStudioModernTextField::GetFilter() const
{
	return Filter;
}

int32 UWidgetStudioModernTextField::GetCharacterLimit() const
{
	return CharacterLimit;
}

void UWidgetStudioModernTextField::SetCharacterLimit(const int32 NewLimit)
{
	CharacterLimit = NewLimit;
	SetText(Text);
}

ETextFieldIconVisibility UWidgetStudioModernTextField::GetIconVisibility() const
{
	return IconVisibility;
}

EIconItem UWidgetStudioModernTextField::GetLeadingIcon() const
{
	return LeadingIcon;
}

EIconItem UWidgetStudioModernTextField::GetTrailingIcon() const
{
	return TrailingIcon;
}

ETextFieldState UWidgetStudioModernTextField::GetState() const
{
	return State;
}

EPalette UWidgetStudioModernTextField::GetBackgroundColor() const
{
	return BackgroundColor;
}

EPalette UWidgetStudioModernTextField::GetContentColor() const
{
	return ContentColor;
}

float UWidgetStudioModernTextField::GetIconSize() const
{
	return UWidgetStudioFunctionLibrary::GetSizeByModifier(SizeModifier, IconSize);
}

void UWidgetStudioModernTextField::SetIconSize(const float NewSize)
{
	IconSize = NewSize;
	SynchronizeProperties();
}

void UWidgetStudioModernTextField::SetLabel(const FText NewLabel)
{
	Label = NewLabel;
	SynchronizeProperties();
}

void UWidgetStudioModernTextField::SetText(const FText NewText)
{
	FText TempText = NewText;

	if (Filter == ETextFilter::NumbersOnly && !TempText.ToString().IsNumeric())
	{
			TempText = Text;
	}

	if (Filter == ETextFilter::TextOnly && UWidgetStudioFunctionLibrary::ContainsNumbers(TempText))
	{
		TempText = Text;
	}

	// Trim whitespace before and after the text
	//TempText = FText::TrimPrecedingAndTrailing(TempText);

	// Trim by character limit
	TempText = UWidgetStudioFunctionLibrary::FilterByCharacterLimit(TempText, CharacterLimit);

	// Set the text
	Text = TempText;

	// Sets the text to the text box
	SynchronizeProperties();

	// Broadcast Change
	OnTextChanged.Broadcast(Text);
}

void UWidgetStudioModernTextField::SetPlaceholderText(const FText NewText)
{
	PlaceholderText = NewText;
	SynchronizeProperties();
}

void UWidgetStudioModernTextField::SetFilter(const ETextFilter NewFilter)
{
	Filter = NewFilter;
	SetText(Text);
}

void UWidgetStudioModernTextField::SetIconVisibility(const ETextFieldIconVisibility NewState)
{
	IconVisibility = NewState;
	SynchronizeProperties();
}

void UWidgetStudioModernTextField::SetLeadingIcon(const EIconItem NewIcon)
{
	LeadingIcon = NewIcon;
	SynchronizeProperties();
}

void UWidgetStudioModernTextField::SetTrailingIcon(const EIconItem NewIcon)
{
	TrailingIcon = NewIcon;
	SynchronizeProperties();
}

void UWidgetStudioModernTextField::SetState(const ETextFieldState NewState)
{
	State = NewState;
}

void UWidgetStudioModernTextField::SetBackgroundColor(const EPalette NewColor)
{
	BackgroundColor = NewColor;
}

void UWidgetStudioModernTextField::SetContentColor(const EPalette NewColor)
{
	ContentColor = NewColor;
}
