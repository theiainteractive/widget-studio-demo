// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WSButtonBase.h"
#include "WSText.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/InvalidationBox.h"
#include "Components/Overlay.h"
#include "Components/ScaleBox.h"
#include "Components/SizeBox.h"

#include "WSModernSwitch.generated.h"

UENUM(BlueprintType)
enum class ELabelPlacement : uint8
{
	Hide				UMETA(DisplayName="Hide"),				
	Left				UMETA(DisplayName="Left"),
	Right				UMETA(DisplayName="Right"),

	LabelPlacement_Max	UMETA(Hidden),
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WIDGETSTUDIORUNTIME_API UWidgetStudioModernSwitch : public UWidgetStudioButtonBase
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
	UHorizontalBox* HBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* Overlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* TrackSizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* TrackScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* TrackOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* TrackDropShadow = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* Track = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* HandleScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* HandleOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* HandleDropShadow = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* Handle = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioText* LabelItem = nullptr;

private:
	
	/* Properties */

	// The text on the label. Label is hidden when empty.
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	FText Label = FText().FromString("Switch");

	// Display the label next to the switch.
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	ELabelPlacement LabelPlacement = ELabelPlacement::Hide;
		
	// The color of the track.
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	EPalette TrackColor = EPalette::PrimaryBackground;

	// The color of the handle.
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	EPalette HandleColor = EPalette::PrimaryAccent;

	// The color of the label.
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	EPalette LabelColor = EPalette::PrimaryBackground;

public:
	
	/* Helpers */

	// Returns the label.
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	FText GetLabel() const;

	// Is the label visible?
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsLabelVisible() const;

	// Returns the color of the track.
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EPalette GetTrackColor() const;
	
	// Returns the color of the handle.
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EPalette GetHandleColor() const;

	// Returns the color of the label.
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EPalette GetLabelColor() const;
	
	/* Modifier Functions */

	// Set the label visibility/placement.
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetLabelPlacement(const ELabelPlacement NewPlacement);

	// Set the labels text.
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetLabel(FText NewLabel);

	// Set the color of the track.
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetTrackColor(EPalette NewColor);

	// Set the color of the handle.
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetHandleColor(EPalette NewColor);

	// Set the color of the label.
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetLabelColor(EPalette NewColor);

};
