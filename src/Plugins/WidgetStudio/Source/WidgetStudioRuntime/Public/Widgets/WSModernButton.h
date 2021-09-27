// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WSButtonBase.h"
#include "WSModernCard.h"
#include "WSIcon.h"
#include "WSText.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/InvalidationBox.h"
#include "Components/Overlay.h"
#include "Components/ScaleBox.h"
#include "Components/Spacer.h"
#include "Components/VerticalBox.h"
#include "WSModernButton.generated.h"

UENUM(Blueprintable, BlueprintType, META=(Tooltip = "The location of the icon in conjunction to the widget."))
enum class EIconPlacement : uint8
{
	Left				UMETA(DisplayName="Left"),
	Right				UMETA(DisplayName="Right"),
	Top					UMETA(DisplayName="Top"),
	Bottom				UMETA(DisplayName="Bottom"),

	IconPlacement_Max	UMETA(Hidden),
};

UENUM(Blueprintable, BlueprintType, META=(Tooltip = "The overall style of the button."))
enum class EButtonStyle : uint8
{
	Solid				UMETA(DisplayName="Solid"),
	Line				UMETA(DisplayName="Outline"),
	Content				UMETA(DisplayName="Content"),

	ButtonStyle_Max		UMETA(Hidden),
};

UENUM(Blueprintable, BlueprintType, META=(Tooltip = "The style of the buttons content."))
enum class EButtonContentStyle : uint8
{
	IconWithLabel		UMETA(DisplayName="Icon with a label"),
	Label				UMETA(DisplayName="Label"),
	Icon				UMETA(DisplayName="Icon"),
	
	ButtonStyle_Max		UMETA(Hidden),
};

USTRUCT(Blueprintable, BlueprintType)
struct FButtonOptions
{
	GENERATED_BODY()

	// General

	/** What content to display on the button. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Studio|Style")
	EButtonContentStyle ContentStyle = EButtonContentStyle::Label;

	/** The text that will display on the button. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Studio", Meta = (EditCondition="ContentStyle == EButtonContentStyle::IconWithLabel || ContentStyle == EButtonContentStyle::Label"))
	FText Text = FText().FromString("New Option");;

	/** The text that will displayed a mouse hovers over the button. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Studio", Meta = (EditCondition="ContentStyle == EButtonContentStyle::IconWithLabel || ContentStyle == EButtonContentStyle::Label"))
	FText ToolTip;

	

	/** The color of the content. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Studio|Color")
	EPalette ContentColor = EPalette::PrimaryContent;

	/** The color of the content when checked. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Studio|Color")
	EPalette CheckedContentColor = EPalette::TertiaryBackground;

	/** How the content should be horizontally aligned. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Studio|Style")
	TEnumAsByte<EHorizontalAlignment> Alignment = HAlign_Center;

	

	/** The icon that will display on the button. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Studio|Icon Options", Meta = (EditCondition="ContentStyle == EButtonContentStyle::IconWithLabel || ContentStyle == EButtonContentStyle::Icon"))
	EIconItem Icon = EIconItem::Heart;

	/** Where the icon should be placed on the button in conjunction with the text. If the text is disabled, this option will be ignored and the icon will be centered. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Studio|Icon Options", Meta = (EditCondition="ContentStyle == EButtonContentStyle::IconWithLabel"))
	EIconPlacement IconPlacement = EIconPlacement::Left;

	/** If set to true, the icon will be colored by the theme's palette. If set to false, the icon's own color will be used. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Studio|Icon Options", Meta = (EditCondition="ContentStyle == EButtonContentStyle::IconWithLabel || ContentStyle == EButtonContentStyle::Icon"))
	bool bColorIconByPalette = true;

	/** The custom icon that will display on the button. If set, this icon will be used instead of the Icon property. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Studio|Icon Options", Meta = (EditCondition="ContentStyle == EButtonContentStyle::IconWithLabel || ContentStyle == EButtonContentStyle::Icon"))
	UTexture2D* CustomIcon = nullptr;
};

/**
 * A fully styled functional button from the Widget Studio. Can be used for all button needs and purposes with minimal setup.
 */
UCLASS(BlueprintType, Blueprintable)
class WIDGETSTUDIORUNTIME_API UWidgetStudioModernButton : public UWidgetStudioButtonBase
{
	GENERATED_BODY()

protected:
	
	#if WITH_EDITOR
		virtual const FText GetPaletteCategory() override;
	#endif
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void SynchronizeProperties() override;
	virtual void InitializeStyling() override;
	virtual void UpdateStyling() override;

	// Internal Functions
	void UpdateContentPositionsAndStyling() const;

	// Internal Properties
	const float BaseDropShadowOpacity = .15;

	UPROPERTY()
	float ShadowLength;

	// Widget Components

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UInvalidationBox* Retainer = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* SizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* Overlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* BackgroundOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* ContentOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* BackgroundScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* DropShadow = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* Background = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UHorizontalBox* HorizontalContent = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UVerticalBox* VerticalContent = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioIcon* IconItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USpacer* SpacerItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioText* TextItem = nullptr;

private:

	// General
	
	/** What content to display on the button. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	EButtonContentStyle ContentStyle = EButtonContentStyle::Label;

	/** How the content should be horizontally aligned. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (EditCondition="ContentStyle == EButtonContentStyle::IconWithLabel || ContentStyle == EButtonContentStyle::Label"))
	TEnumAsByte<EHorizontalAlignment> ContentAlignment = HAlign_Center;

	
	// Text

	/** The text that will display on the button. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Text", Meta = (EditCondition="ContentStyle == EButtonContentStyle::IconWithLabel || ContentStyle == EButtonContentStyle::Label"))
	FText Text = FText().FromString("Button");

	// Icon

	/** The icon that will display on the button. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Icon", Meta = (EditCondition="ContentStyle == EButtonContentStyle::IconWithLabel || ContentStyle == EButtonContentStyle::Icon"))
	EIconItem Icon = EIconItem::Heart;

	/** Where the icon should be placed on the button in conjunction with the text. If the text is disabled, this option will be ignored and the icon will be centered. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Icon", Meta = (EditCondition="ContentStyle == EButtonContentStyle::IconWithLabel"))
	EIconPlacement IconPlacement = EIconPlacement::Left;

	/** If set to true, the icon will be colored by the theme's palette. If set to false, the icon's own color will be used. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Icon", Meta = (EditCondition="ContentStyle == EButtonContentStyle::IconWithLabel || ContentStyle == EButtonContentStyle::Icon"))
	bool bColorIconByPalette = true;

	/** Use an icon not found in the icon library. Clear this when you want to use the icons from the icon library. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Icon", Meta = (EditCondition="ContentStyle == EButtonContentStyle::IconWithLabel || ContentStyle == EButtonContentStyle::Icon"))
	UTexture2D* CustomIcon;

	// Colors

	/** The color of the background when the button is NOT in a checked state. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette StandardBackgroundColor = EPalette::TertiaryBackground;

	/** The color of the background when the button IS in a checked or pressed state. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette CheckedBackgroundColor = EPalette::PrimaryAccent;

	/** The color of the content (Text/Icon) when the button is NOT in a checked state. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette StandardContentColor = EPalette::PrimaryContent;

	/** The color of the content (Text/Icon) when the button IS in a checked or pressed state. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette CheckedContentColor = EPalette::TertiaryBackground;

	// Styling

	/** Enables the drop shadow effect when hovered. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	bool bEnableDropShadow = true;

	/** How the button will be displayed. The rounded border radius is controlled in the Project Settings under Widget Studio. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	EButtonStyle ButtonStyle = EButtonStyle::Solid;

	/** How the corners of the button will appear visually. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style", Meta = (EditCondition="ButtonStyle == EButtonStyle::Solid || ButtonStyle == EButtonStyle::Line"))
	ECornerStyle CornerStyle = ECornerStyle::Rounded;

	/** How the buttons shadow will be displayed. Will be displayed upon mouseover. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style", Meta = (EditCondition="ButtonStyle == EButtonStyle::Solid || ButtonStyle == EButtonStyle::Line"))
	EShadowStyle ShadowStyle = EShadowStyle::Small;

	/** The amount of padding between the content and the edges of the button. If both the Text & Button are displayed, the padding on the left and right side will be tripled.  */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	float ContentPadding = 10;

public:

	// Helpers

	/** Is the text valid? */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Text")
	bool IsTextValid() const;

	/** Returns the text displayed on the button */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Text")
	FText GetText() const;


	/** Is the icon valid? */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Icon")
	bool IsIconValid() const;

	/** Returns the icon displayed on the button. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Icon")
	EIconItem GetIcon() const;

	/** Returns true if the color is being colored by the theme's palette. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Icon")
	bool IsIconColoredByPalette() const;

	/** Returns true if a custom icon has been set and is being used. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsUsingCustomIcon() const;

	/** Returns the custom icon. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	UTexture2D* GetCustomIcon() const;
	

	/** Returns the standard background color of the button. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Color")
	EPalette GetStandardBackgroundColor() const;

	/** Returns the checked background color of the button. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Color")
	EPalette GetCheckedBackgroundColor() const;

	/** Returns the standard content (Icon/Text) color of the button. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Color")
	EPalette GetStandardContentColor() const;

	/** Returns the checked content (Icon/Text) color of the button. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Color")
	EPalette GetCheckedContentColor() const;

	/** Returns the placement of the icon within the button. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Style")
	EIconPlacement GetIconPlacement() const;

	/** Returns the corner style of the button. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Style")
	ECornerStyle GetCornerStyle() const;

	/** Returns the style of the button. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Style")
	EButtonStyle GetButtonStyle() const;

	/** Returns the content style of the button. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Style")
	EButtonContentStyle GetContentStyle() const;


	// Modifiers

	/** Set the text displayed. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|Text")
	void SetText(FText NewText);

	/** Set the placement of the content on the button. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetAlignment(TEnumAsByte<EHorizontalAlignment> NewAlignment);

	
	/** Set the icon displayed. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|Icon")
	void SetIcon(EIconItem NewIcon);

	/** Set the placement of the icon on the button. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|Icon")
	void SetIconPlacement(EIconPlacement NewIconPlacement);

	/** Set the icon coloring method. If set to true, the icon will be colored by the palette. If set to false, the icon will use it's default color. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|Icon")
	void SetColorIconByPalette(bool NewState);

	/** Sets the icon to use a custom icon. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetCustomIcon(UTexture2D* NewCustomIcon);

	/** Clears the set custom icon and automatically switches the widget to use the icon library. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void ClearCustomIcon();
	

	/** Set the color of the background when the button is NOT in a checked state. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|Color")
	void SetStandardBackgroundColor(EPalette NewColor);

	/** Set the color of the background when the button IS in a checked state. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|Color")
	void SetCheckedBackgroundColor(EPalette NewColor);

	/** Set the color of the content (Text/Icon) when the button is NOT in a checked state. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|Color")
	void SetStandardContentColor(EPalette NewColor);

	/** Set the color of the content (Text/Icon) when the button IS in a checked state. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|Color")
	void SetCheckedContentColor(EPalette NewColor);


	/** Set how the corners of the button will appear visually. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|Style")
	void SetCornerStyle(ECornerStyle NewCornerStyle);

	/** Set how the style of the button will appear visually. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|Style")
	void SetButtonStyle(EButtonStyle NewButtonStyle);

	/** Set how the content of the button will appear visually. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|Style")
	void SetContentStyle(EButtonContentStyle NewContentStyle);
};
