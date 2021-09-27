// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WSBase.h"
#include "WSTextBlock.h"
#include "Components/InvalidationBox.h"
#include "Components/TextBlock.h"

#include "WSText.generated.h"

/**
 * A custom text widget setup to work with the Typography Styling.
 */

UCLASS(BlueprintType, Blueprintable)
class WIDGETSTUDIORUNTIME_API UWidgetStudioText : public UWidgetStudioBase
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

	UPROPERTY()
	FSlateFontInfo FontInfo;

	// Widget Components

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UInvalidationBox* Retainer = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioTextBlock* TextItem = nullptr;
	
private:
	// Widget Studio

	/** The text that is displayed. */
	UPROPERTY(EditAnywhere, Category="Widget Studio", Meta=(MultiLine=true))
	FText Text = FText::FromString("Placeholder");

	/** The font type that'll govern the style of the text.*/
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	EFontType Type = EFontType::Body1;

	/** Set the case style of the text that is displayed.*/
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	EFontCase Case = EFontCase::Sentence;

	/** The growth direction and alignment of the text.*/
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	TEnumAsByte<ETextJustify::Type> Justification = ETextJustify::Left;

	/** A custom font style. Useful for one-off styles. */
	UPROPERTY(EditAnywhere, Category="Widget Studio", Meta = (EditCondition = "Type == EFontType::Custom"))
	FFontStyle CustomStyle;

	
	/* Text Wrap */

	/** Automatically wraps the text to the next line when it reaches the end of available horizontal space.*/
	UPROPERTY(EditAnywhere, Category="Widget Studio|Text Wrap")
	bool AutoWrapText = false;
	
	/** Manually wraps the text at the given distance. Ignored if set to 0.*/
	UPROPERTY(EditAnywhere, Category="Widget Studio|Text Wrap")
	float WrapTextAt = 0.0;

	/** How the text should wrap.*/
	UPROPERTY(EditAnywhere, Category="Widget Studio|Text Wrap")
	ETextWrappingPolicy WrappingPolicy = ETextWrappingPolicy::DefaultWrapping;

	
	/* Color */

	/** The color of the text.*/
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	EPalette Color = EPalette::PrimaryContent;

public:
	
	// Helpers

	/** Returns the displayed text.*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	FText GetText() const;

	/** Returns true if the text is empty.*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsTextEmpty() const;

	/** Returns the justification of the text.*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	ETextJustify::Type GetJustification() const;

	/** Returns how long the text is (in characters).*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	int32 Length() const;

	/** Returns the text type used.*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EFontType GetType() const;

	/** Returns the custom text style.*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	FFontStyle GetCustomStyle() const;

	/** Returns true if the text is automatically being wrapping.*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsAutoWrappingText() const;

	/** Returns the set distance to wrap text at.*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float GetWrapTextAt() const;

	/** Returns the set wrapping policy.*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	ETextWrappingPolicy GetWrapPolicy() const;

	/** Returns the set case of the text.*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EFontCase GetCase() const;


	// Modifier Functions

	/** Set the text that will be displayed by the widget.*/
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetText(FText NewText);

	/** Set the growth direction and alignment of the text.*/
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetJustification(ETextJustify::Type NewJustification);

	/** Set the color of the text.*/
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetColor(EPalette NewColor);

	/** Set the font type that'll govern the style of the text.*/
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetType(EFontType NewType);

	/** Set the custom font style. Will only take effect if the Font Type is set to Custom. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetCustomStyle(FFontStyle NewStyle);

	/** Set the text to automatically wrap or not.*/
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetAutoWrapText(bool NewState);

	/** Set where to wrap the text at. Ignored if set to 0.*/
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetWrapTextAt(float NewWrapAt);

	/** Set how the text should be wrapped.*/
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetWrapPolicy(ETextWrappingPolicy NewPolicy);

	/** Set the case style of the text.*/
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetCase(EFontCase NewCase);
};