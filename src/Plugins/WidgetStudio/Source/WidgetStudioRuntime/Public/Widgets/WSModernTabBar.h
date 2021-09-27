// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WSButtonGroup.h"
#include "WSContainer.h"
#include "Components/ScrollBox.h"
#include "Components/UniformGridPanel.h"

#include "WSModernTabBar.generated.h"

UENUM(BlueprintType)
enum class ETabBarSelectionStyle : uint8
{
	Full						UMETA(DisplayName="Full"),
	Slim						UMETA(DisplayName="Slim"),
	Hidden						UMETA(DisplayName="Hidden"),

	TabBarSelectionStyle_Max	UMETA(Hidden),
};

/**
 * A modern styled tab bar. Useful for multi-tab navigation or selecting an option from many.
 */

UCLASS(BlueprintType, Blueprintable)
class WIDGETSTUDIORUNTIME_API UWidgetStudioModernTabBar : public UWidgetStudioContainer
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
	virtual void ConstructOption(FButtonOptions Option) override;
	virtual void ClearOptions() override;
	virtual void ConstructButtonGroup();
	
	// Widget Components

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UInvalidationBox* Retainer = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* SizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScrollBox* ScrollBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* Overlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioButtonGroup* ButtonGroup = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* BackgroundScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* BackgroundOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* BackgroundDropShadow = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* Background = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* SelectionOverlaySize = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* SelectionOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UUniformGridPanel* Grid = nullptr;

private:

	// General Properties

	/** The direction to grow the bar.
	 * Requires a Re-Compile to take effect.
	 */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	TEnumAsByte<EOrientation> Orientation = Orient_Horizontal;


	// Scroll Properties

	/**
	 * When enabled, the Tab Bar will be scrollable. 
	 * Scrollability starts when the Tab Bar's dimensions exceeds the parent widgets or set Size Constraints.
	 * Note: When enabled, the Tab Bar cannot fill the parent widgets size and will be dependent on it's own dimensions.
	 */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Scrolling")
	bool bEnableScrolling = false;
	
	/** Constrains the size of the Tab Bar.
	 * - If the tab bar extends beyond the giving size constraint, the tab bar will become scrollable.
	 * - Size and scroll direction is dependent on set orientation.
	 * - If set to 0, or Horizontal/Vertical Alignment set to Align Fill, size constraint will be ignored.
	 */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Scrolling", Meta = (ClampMin = "0", UIMin = "0", EditCondition="bEnableScrolling"))
	float SizeConstraint = 0.f;

	/** Set the scroll visibility of the scroll bar. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Scrolling", Meta = (EditCondition="bEnableScrolling"))
	ESlateVisibility ScrollBarVisibility = ESlateVisibility::Visible;

	// Style Properties

	/** Enables the drop shadow effect when hovered. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	bool bEnableDropShadow = true;

	/** How the corners of the button will appear visually. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	ECornerStyle CornerStyle = ECornerStyle::Rounded;

	/** The style of the selection indicator. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	ETabBarSelectionStyle SelectionStyle = ETabBarSelectionStyle::Full;


	// Color Properties

	/** The color of the background. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette BackgroundColor = EPalette::TertiaryBackground;

	/** The color of the content. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette ContentColor = EPalette::PrimaryContent;

	/** The color of the selection. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette SelectionColor = EPalette::PrimaryAccent;


	// State Properties

	/** Set if the tab bar is selectable or not. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|State")
	bool bSelectable = true;

public:

	// Helpers
	
	/** Returns the Tab Bar orientation. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	TEnumAsByte<EOrientation> GetOrientation() const;

	/** Returns the Tab Bar corner style. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	ECornerStyle GetCornerStyle() const;

	/** Returns the Tab Bar selection style. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	ETabBarSelectionStyle GetSelectionStyle() const;

	/** Returns the Tab Bar background color. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EPalette GetBackgroundColor() const;

	/** Returns the Tab Bar content color. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EPalette GetContentColor() const;

	/** Returns the Tab Bar selection color. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EPalette GetSelectionColor() const;

	/** Returns the Tab Bar checkable state. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsSelectable() const;


	// Scrolling Helper

	/** Returns if the Tab Bar is scrollable or not. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float IsScrollable() const;

	/** Returns the current size constraint value.. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float GetSizeConstraint() const;

	/** Returns the current scroll bar visibility. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	ESlateVisibility GetScrollbarVisibility() const;

	// Modifiers
	
	/** Set the current index. */
	virtual void SetCurrentIndex(int32 Index) override;

	/** Set the grow direction. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetOrientation(TEnumAsByte<EOrientation> NewOrientation);

	/** Set how the corner style looks. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetCornerStyle(ECornerStyle NewStyle);

	/** Set the color of the background. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetBackgroundColor(EPalette NewColor);

	/** Set the color of the content.*/
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetContentColor(EPalette NewColor);

	/** Set the color of the selection.*/
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetSelectionColor(EPalette NewColor);

	/** Set the selectable state.*/
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetSelectable(bool NewState);


	// Scrolling Modifiers

	/** Enable or disable the scrollability of the Tab Bar. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetScrollable(bool NewState);

	/** Constrains the size of the Tab Bar. \n
	* If the tab bar extends beyond the giving size constraint, the tab bar will become scrollable. \n \n
	* Size and scroll direction is dependent on set orientation. \n \n
	* If set to 0, size constraint will be ignored.
	*/
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetSizeConstraint(float NewSize);

	/** Set the scroll visibility of the scroll bar. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetScrollbarVisibility(ESlateVisibility NewVisibility);
};
