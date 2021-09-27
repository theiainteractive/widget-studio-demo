// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WSContainer.h"
#include "WSModernTabBar.h"
#include "Components/InvalidationBox.h"
#include "Components/MenuAnchor.h"

#include "WSModernComboBox.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WIDGETSTUDIORUNTIME_API UWidgetStudioModernComboBox : public UWidgetStudioContainer
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
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	void OnMenuButtonSelected(int32 Index, FButtonOptions Option);

	UFUNCTION()
	UUserWidget* ConstructMenu() const;

	/* * Widget Components */

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UInvalidationBox* Retainer = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UVerticalBox* VBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioText* LabelItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* ContentSizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* ContentScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* ContentOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* DropShadow = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* Background = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UHorizontalBox* HBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioIcon* IconItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioText* TextItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* ArrowScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioIcon* ArrowItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UMenuAnchor* MenuAnchor = nullptr;

private:

	// General Properties

	/** The label that appears above the Combo Box. If empty, the label will be hidden. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	FText Label;

	/** The text displayed when an option isn't selected (Current index = -1) */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	FText PlaceholderText = FText().FromString("Select option");

	// Dropdown Menu

	/** The maximum height of the dropdown menu. If the number of options exceeds this height, the menu will become scrollable. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|State")
	float MenuMaxHeight = 225;

	
	// Colors Properties

	/** The color of the background. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette BackgroundColor = EPalette::TertiaryBackground;

	/** The color of the content. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette ContentColor = EPalette::PrimaryContent;

	/** The color of the overlay. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette SelectionColor = EPalette::PrimaryAccent;

	/** The color of the label. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette LabelColor = EPalette::PrimaryContent;

	
	// Style Properties
	
	/** How the corners of the button will appear visually. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	ECornerStyle CornerStyle = ECornerStyle::Rounded;

	// Scroll Properties
	
	/** The maximum height of the dropdown menu. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	float MenuHeight = 225;
	
	// State Properties
	
	/** Are the options in this Combo Box selectable? Disabling this option will have the Combo Box act as a drop down menu instead. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|State")
	bool bIsSelectable = true;


	
public:
	// Overrides
	virtual void SetCurrentIndex(int32 Index) override;

	// Helpers

	/** Returns the label's text. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	FText GetLabel() const;

	/** Returns the placeholder's text. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	FText GetPlaceholderText() const;

	/** Returns the corner style. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	ECornerStyle GetCornerStyle() const;
	
	/** Returns the background color. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Color")
	EPalette GetBackgroundColor() const;

	/** Returns the content color (Text/Icon). */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Color")
	EPalette GetContentColor() const;

	/** Returns the selection color. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Color")
	EPalette GetSelectionColor() const;

	/** Returns the label color. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Color")
	EPalette GetLabelColor() const;

	/** Are the Combo Box items selectable? */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Color")
	bool IsSelectable() const;

	
	// Modifiers

	/** Sets the label's text. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetLabel(FText NewLabel);

	/** Sets the placeholder's text. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetPlaceholderText(FText NewText);

	/** Sets the how the corners look. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetCornerStyle(ECornerStyle NewStyle);

	/** Set the background color. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetBackgroundColor(EPalette NewColor);

	/** Set the content color. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetContentColor(EPalette NewColor);

	/** Set the selection color. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetSelectionColor(EPalette NewColor);

	/** Set the label color. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetLabelColor(EPalette NewColor);

	/** Set the selectable state of the Combo Box. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetSelectable(bool NewState);

};

