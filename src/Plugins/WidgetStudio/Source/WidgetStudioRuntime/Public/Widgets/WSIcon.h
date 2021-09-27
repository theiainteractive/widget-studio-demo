/** Fill out your copyright notice in the Description page of Project Settings. */

#pragma once

#include "CoreMinimal.h"

#include "WSBase.h"
#include "Components/Image.h"
#include "Components/InvalidationBox.h"
#include "Components/ScaleBox.h"
#include "Components/SizeBox.h"

#include "WSIcon.generated.h"

/**
 * A pre-setup widget that works with power of two icons. For example, 64x64.
 */
UCLASS(BlueprintType, Blueprintable)
class WIDGETSTUDIORUNTIME_API UWidgetStudioIcon : public UWidgetStudioBase
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
	UScaleBox* ScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* IconItem = nullptr;

private:
	/* Properties  */

	/** The icon to display from the Icon Set. */
	UPROPERTY(EditAnywhere, Category="Widget Studio", Meta = (EditCondition="CustomIcon == nullptr"))
	EIconItem Icon = EIconItem::Heart;

	/** The size of the icon. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (ClampMin = "16", UIMin = "16"))
	float Size = 24;

	/** Use an icon not found in the icon library. Clear this when you want to use the icons from the icon library. */
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	UTexture2D* CustomIcon;

	/** If true, the icon will utilize the theme's palette. If false, the icon's own color will be utilized. */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	bool bUsePaletteColor = true;
	
	/** The color of the icon. */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	EPalette Color = EPalette::PrimaryContent;

public:
	/* Helpers */

	/** Returns the icon used from the icon library. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EIconItem GetIcon() const;

	/** Returns the color used for the icon. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EPalette GetColor() const;

	/** Returns true if a custom icon has been set and is being used. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsUsingCustomIcon() const;

	/** Returns the custom icon. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	UTexture2D* GetCustomIcon() const;

	/** Returns the size of the icon. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float GetSize() const;

	/** Returns the true if using the theme's palette. Returns false if the icon's own color is being used. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float IsUsingPaletteColor() const;

	/* Modifiers */

	/** Sets the icon to use an icon from the Icon Set. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetIcon(EIconItem NewIcon);

	/** Sets the color of the icon. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetColor(EPalette NewColor);

	/** Sets the icon to use a custom icon. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetCustomIcon(UTexture2D* NewCustomIcon);

	/** Clears the set custom icon and automatically switches the widget to use the icon library. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void ClearCustomIcon();

	/** Sets the size of the icon. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetSize(float NewSize);

	/** Set to true to use the theme's palette to color the icon. Set to false to use the icon's own color. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetUsePaletteColor(bool NewState);
};
