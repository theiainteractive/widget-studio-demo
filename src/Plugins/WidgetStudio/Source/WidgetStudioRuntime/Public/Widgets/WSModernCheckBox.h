// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WSButtonBase.h"
#include "WSIcon.h"
#include "WSText.h"
#include "WidgetStudioRuntime.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/InvalidationBox.h"
#include "Components/Overlay.h"
#include "Components/SizeBox.h"
#include "Components/Spacer.h"

#include "WSModernCheckBox.generated.h"

UENUM(Blueprintable, BlueprintType, META=(Tooltip = "The styling of the checkbox."))
enum class ECheckboxStyle : uint8
{
	Modern				UMETA(DisplayName="Modern"),
	Classic				UMETA(DisplayName="Classic"),
	
	CheckboxStyle_Max	UMETA(Hidden),
};

UENUM(Blueprintable, BlueprintType, META=(Tooltip = "Choose between inclusive or exclusive."))
enum class EClusivity : uint8
{
	Inclusive				UMETA(DisplayName="Inclusive"),
	Exclusive				UMETA(DisplayName="Exclusive"),
	
	Clusivity_Max	UMETA(Hidden),
};


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class WIDGETSTUDIORUNTIME_API UWidgetStudioModernCheckBox : public UWidgetStudioButtonBase
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
	
	/* Widget Components */

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UInvalidationBox* Retainer = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* SizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* BackgroundScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* Overlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* BackgroundOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* DropShadow = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* Background = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* CheckScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* BackgroundOutline = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UHorizontalBox* HBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* CheckOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* CheckBackground = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* CheckOutline = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* IconItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioText* TextItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USpacer* SpacerItem = nullptr;

private:
	
	// The label displayed on the checkbox.
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	FText Label = FText().FromString("CheckBox");
	
	// The icon displayed on the checkbox when checked (on/true).
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	EIconItem Icon = EIconItem::Check;

	// The stylization of the checkbox.
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	ECheckboxStyle Style = ECheckboxStyle::Modern;
	
	// Choose if the checkbox will be used for selecting a single or multiple options.
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	EClusivity Clusivity = EClusivity::Inclusive;

	/* Colors */

	// The color of the background. Only available when the CheckBox style is set to modern.
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette BackgroundColor = EPalette::TertiaryBackground;

	// The color of the content (Text/Icon) when the button is NOT in a checked state.
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette ContentColor = EPalette::PrimaryContent;

	// The color of the checkbox when the button IS in a checked.
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette CheckedColor = EPalette::PrimaryAccent;

public:

	/* Helpers */

	// Returns the text.
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	FText GetLabel() const;

	// Returns the icon.
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	EIconItem GetIcon() const;
	
	// Returns the background color.
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Color")
	EPalette GetBackgroundColor() const;

	// Returns the content (Text/Icon).
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Color")
	EPalette GetContentColor() const;

	// Returns the checked color.
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Color")
	EPalette GetCheckedColor() const;

	// Returns the selection method.
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Color")
	EClusivity GetSelectionMethod() const;

	
	/* Modifiers */

	// Sets the label displayed on the checkbox.
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetLabel(FText NewLabel);

	// Sets the icon displayed on the checkbox when checked (on/true).
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetIcon(EIconItem NewIcon);

	// Set the color of the background. Only available when the CheckBox style is set to modern.
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|Color")
	void SetBackgroundColor(EPalette NewColor);

	// Set the color of the content (Text/Icon) when the button IS in a checked state.
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|Color")
	void SetContentColor(EPalette NewColor);

	// Set the color of the checkbox when checked.
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|Color")
	void SetCheckedColor(EPalette NewColor);

	/* The button selection method.
	* Inclusive: Multiple buttons can be selected at a time.
	* Exclusive: Only a single button can be selected at a time.
	*/
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetSelectionMethod(EClusivity NewState);

};
