/** Fill out your copyright notice in the Description page of Project Settings. */

#pragma once

#include "CoreMinimal.h"
#include "WSText.h"

#include "Components/EditableTextBox.h"
#include "Components/EditableText.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ScaleBox.h"
#include "Components/SizeBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Widgets/WSBase.h"
#include "Types/SlateEnums.h"
#include "Types/WSEnums.h"
#include "WSModernSlider.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWFSliderDelegate, float, NewValue);

/**
 * A modern styled slider. Useful for setting a value between a minimum and a maximum.
 */
UCLASS(BlueprintType, Blueprintable)
class WIDGETSTUDIORUNTIME_API UWidgetStudioModernSlider : public UWidgetStudioBase
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

	/* Internal Functions */

	/** Filter Value based on set properties */
	UFUNCTION()
	float FilterValue(float InValue) const;

	UFUNCTION()
	void OnTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod);

	/* Widget Components */

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UVerticalBox* VBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* SizeBox = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UHorizontalBox* HBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* Overlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* TrackScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* TrackOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* Track = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* TrackDropShadow = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* FillTrackSizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* FillTrack = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* HandleScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* HandleSizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* HandleOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* HandleDropShadow = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* Handle = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USlider* SliderItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* TextInputSizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UEditableTextBox* TextInput = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioText* LabelItem = nullptr;

private:
	/* Bindings */

	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWFSliderDelegate OnValueChanged;

	/* Internal Functions */

	/** Calls SetValue when built-in slider is changed. No NOT touch. */
	UFUNCTION()
	void SyncSliderValue(float NewValue);
	
	/* Properties */

	/** The label of the text field. If empty, the label will be hidden. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	FText Label;

	/** Allow users to interact with the slider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	bool bEnableInput = true;

	/** Display the handle of the slider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	bool bDisplayHandle = true;

	/** Display the value next to the slider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	bool bDisplayValue = true;
	
	/** The current value of the slider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	float Value = 50.f;

	/** The minimum value of the slider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	float Min = 0.f;

	/** The maximum value of the slider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	float Max = 100.f;

	/** The step size value of the slider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio", Meta = (ClampMin = "0", UIMin = "0"))
	float StepSize = 1.f;

	/** The value type of the slider. Note, change the Step Size to a decimal value for this to properly take effect. */ 
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	EEnumeration Enumeration = EEnumeration::Int;

	/* Color */

	/** Instead of using the accent color, the slider color will be based on thresholds. [0] Friendly -> Hostile [1] */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	bool bColorBasedOnValue = false;

	/** Reverses the color based on value's order. Changes to [0] Hostile -> Friendly [1] */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	bool bReverseColorOrder = false;
	
	/** The color of the slider track. */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	EPalette TrackColor = EPalette::PrimaryBackground;

	/** The color of the slider handle. */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color", Meta = (EditCondition="bColorBasedOnValue == false"))
	EPalette HandleColor = EPalette::PrimaryAccent;

	/** The color of both the Label and the Value. */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	EPalette TextColor = EPalette::PrimaryContent;

	/* Hidden */
	UPROPERTY()
	bool bLockIncrement = false;

	UPROPERTY()
	bool bLockDecrement = false;

public:
	
	/* Helpers */

	/** Returns the Label Text. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	FText GetLabelText() const;

	/** Returns the current slider value. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float GetValue() const;

	/** Returns the minimum slider value. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float GetMinValue() const;

	/** Returns the maximum slider value. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float GetMaxValue() const;

	/** Returns the step size of the slider. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float GetStepSize() const;

	/** Is the value visible? */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsValueVisible() const;

	/** Is the handle visible? */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsHandleVisible() const;

	/** Returns true if the color is based on value. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsColorBasedOnValue() const;

	/** Returns true if the color based on value order is reversed. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsColorBasedOnValueReversed() const;

	/** Returns the enumeration type. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EEnumeration GetEnumerationType() const;

	/** Can the user interact with the slider? */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsInputEnabled() const;

	/** Can the user increase the value? */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsIncrementLocked() const;

	/** Can the user decrease the value? */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsDecrementLocked() const;

	
	/* Modifier Functions */

	/** Set the text displayed on the Label. If the Text value is empty, the Label will be hidden. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetLabelText(FText NewText);

	/** Set the value of the slider. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetValue(float NewValue, bool bTriggerValueChangedEvent = true);

	/** Set the minimum value of the slider. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetMinValue(float NewValue);

	/** Set the maximum value of the slider. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetMaxValue(float NewValue);

	/** Set the step size of the slider. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetStepSize(float NewValue);

	/** Set the values visibility. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetValueVisibility(bool bNewState);

	/** Set the handles visibility. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetHandleVisibility(bool bNewState);

	/** Set the color to be based on value (true) or to use the handle color (false). */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetColorBasedOnValue(bool bNewState);

	/** Reverses the color based on value's order. Changes to [0] Hostile -> Friendly [1]. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetReverseColorOrder(bool bNewState);

	/** Set the enumeration type. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetEnumerationType(EEnumeration NewType);

	/** Allow or disallow the user to interact with the slider. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetInputState(bool NewState);

	/** Allow or disallow the user to increase the value. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetLockIncrement(bool NewState);

	/** Allow or disallow the user to decrease the value. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetLockDecrement(bool NewState);
};
