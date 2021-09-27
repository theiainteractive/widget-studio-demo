/** Fill out your copyright notice in the Description page of Project Settings. */

#pragma once

#include "CoreMinimal.h"
#include "WidgetStudioRuntime.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Theme/WSIconSet.h"
#include "Theme/WSTheme.h"
#include "Theme/WSTypography.h"
#include "WSFunctionLibrary.generated.h"

/**
 * A collection of useful Widget Studio functions.
 */
UCLASS(BlueprintType, Blueprintable)
class WIDGETSTUDIORUNTIME_API UWidgetStudioFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/* Color */

	/** Returns the FLinearColor value of the given Palette Color. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Color")
	static FLinearColor GetColorFromPalette(EPalette Color);

	/** Returns friendly to hostile color based on the value given. The default color order is --- [0] Friendly -> Warning -> Error [1]
	 * @param ValuePercent The 0 - 1 value in which to grade the color value.
	 * @param bReverseOrder Reverses the color order. If set to true, the order will be [0] Error -> Warning -> Friendly [1]
	 */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Color", Meta = (MinClamp = "0.0", MaxClamp = "0.0"))
	static FLinearColor GetColorBasedValue(float ValuePercent, bool bReverseOrder = false);

	/** Returns friendly to hostile color based on the value given. The default color order is --- [0] Friendly -> Warning -> Error [1]
	** @param ValuePercent The 0 - 1 value in which to grade the color value.
	* @param bReverseOrder Reverses the color order. If set to true, the order will be [0] Error -> Warning -> Friendly [1]
	*/
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Color", Meta = (MinClamp = "0.0", MaxClamp = "0.0"))
	static EPalette GetPaletteBasedValue(float ValuePercent, bool bReverseOrder = false);

	
	/* Style */

	/** Returns the texture from the icon library. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Style")
	static UTexture2D* GetTextureFromIconLibrary(EIconItem Icon);

	/** Returns the control dimensions from the Widget Studio Subsystem. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Style")
	static FVector2D GetControlDimensions();

	/** Modifies and returns the given size based upon the modifier.
	* - Mini: 55%
	* - Tiny: 70%
	* - Small: 85%
	* - Regular: 100%
	* - Large: 115%
	* - Big: 130%
	* - Huge: 145%
	* - Massive: 160%
	*/
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Style")
	static float GetSizeByModifier(const ESizeModifier Modifier, const float InSize);

	/** Returns the border radius from the Widget Studio Subsystem */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Style")
	static float GetBorderRadius();

	/** Returns the current theme in use by the Widget Studio. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Style")
	static UWidgetStudioTheme* GetCurrentTheme();

	/** Returns the current typography in use by the Widget Studio. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Style")
	static UWidgetStudioTypography* GetCurrentTypography();

	/** Returns the current icon set in use by the Widget Studio. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Style")
	static UWidgetStudioIconSet* GetCurrentIconSet();

	/** Sets the theme to use with the Widget Studio. Widget Studio widgets will dynamically adjust. Returns true if theme was successfully applied. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Style")
	static bool SetTheme(const TSoftObjectPtr<UWidgetStudioTheme> NewTheme);

	/** Sets the typography to use with the Widget Studio. Widget Studio widgets will dynamically adjust. Returns true if typography was successfully applied. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Style")
	static bool SetTypography(const TSoftObjectPtr<UWidgetStudioTypography> NewTypography);

	/** Sets the icon set to use with the Widget Studio. Widget Studio widgets will dynamically adjust. Returns true if icon set was successfully applied. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Style")
	static bool SetIconSet(const TSoftObjectPtr<UWidgetStudioIconSet> NewIconSet);


	/* Animation */
	
	/** Smoothly interpolate the widget's color and opacity. Setting Speed to 0 will skip the interp. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Function Library|Animation")
	static void InterpWidgetColor(UUserWidget* Widget, EPalette TargetColor, float Speed);

	/** Smoothly interpolate the image widget's color and opacity. Setting Speed to 0 will skip the interp. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Function Library|Animation")
	static void InterpImageColor(UImage* Widget, EPalette TargetColor, float Speed);

	/** Smoothly interpolate the text widget's color and opacity. Setting Speed to 0 will skip the interp. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Function Library|Animation")
	static void InterpTextColor(UTextBlock* Widget, EPalette TargetColor, float Speed);

	/** Smoothly interpolate the text widget's color and opacity. Setting Speed to 0 will skip the interp. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Function Library|Animation")
	static void InterpEditableTextBoxColor(UEditableTextBox* Widget, EPalette TargetColor, float Speed);

	/** Smoothly interpolate the dimension overrides of the size box. Setting Speed to 0 will skip the interp. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Function Library|Animation")
	static void InterpSizeBoxOverrides(USizeBox* Widget, float TargetWidth, float TargetHeight, float Speed, bool bIgnoreZero = true);

	/** Smoothly interpolate the dimension minimum overrides of the size box. Setting Speed to 0 will skip the interp. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Function Library|Animation")
	static void InterpSizeBoxMinOverrides(USizeBox* Widget, float TargetWidth, float TargetHeight, float Speed);

	/** Smoothly interpolate the widgets render translation. Setting Speed to 0 will skip the interp. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Function Library|Animation")
	static void InterpWidgetTranslation(UWidget* Widget, const FVector2D TargetTranslation, const float Speed);

	/** Smoothly interpolate the widgets render translation. Setting Speed to 0 will skip the interp. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Function Library|Animation")
	static void InterpWidgetScale(UWidget* Widget, const FVector2D TargetScale, const float Speed);

	/** Smoothly interpolate the widgets render rotation. Setting Speed to 0 will skip the interp. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Function Library|Animation")
	static void InterpWidgetRotation(UWidget* Widget, const float TargetRotation, const float Speed);

	/** Smoothly interpolate the widgets render opacity. Setting Speed to 0 will skip the interp. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Function Library|Animation")
	static void InterpWidgetOpacity(UWidget* Widget, float TargetOpacity, float Speed);

	/** Smoothly interpolate the brush image size. Setting Speed to 0 will skip the interp. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Function Library|Animation")
	static void InterpBrushImageSize(UImage* Image, float TargetWidth, float TargetHeight, float Speed);


	/* Material */

	/** Return a reference to the Rounded Background Material */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Material")
	static UMaterialInterface* GetRoundedBackgroundMaterial();

	/** Return a reference to the Rounded Outline Material */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Material")
	static UMaterialInterface* GetRoundedOutlineMaterial();

	/** Return a reference to the Rounded Shadow Material */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Material")
	static UMaterialInterface* GetRoundedShadowMaterial();

	
	/* Typography */

	/** Return the current Typeface */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Typography")
	static UFont* GetTypefaceFromTypography();

	/** Checks if the text contains any number. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Typography")
	static bool ContainsNumbers(FText InText);

	/** Checks if the text contains any number. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Typography")
	static FText FilterByCharacterLimit(const FText InText, const int32 CharacterLimit);

	/** Create Font Info from Typography type */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Function Library|Typography")
	static FSlateFontInfo ConstructFontInfoFromTypography(const EFontType FontType);

	/** Create Font Info */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Function Library|Typography")
	static FSlateFontInfo ConstructFontInfoFromStyle(const FFontStyle FontStyle);

	/** Returns the type scale from the Typography. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Typography")
	static FFontStyle GetTypeScaleFromTypography(const EFontType FontType);

	/** Returns the font weight name. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Function Library|Typography")
	static FName GetFontWeightName(EFontWeight FontWeight);
};
