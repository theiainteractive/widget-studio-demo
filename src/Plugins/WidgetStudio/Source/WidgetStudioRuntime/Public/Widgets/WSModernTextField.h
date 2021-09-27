/** Fill out your copyright notice in the Description page of Project Settings. */

#pragma once

#include "CoreMinimal.h"

#include "WSModernCard.h"
#include "WSBase.h"
#include "Components/HorizontalBox.h"
#include "Components/InvalidationBox.h"
#include "Components/Overlay.h"
#include "Components/ScaleBox.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

#include "WSModernTextField.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWFTextFieldTextChangedDelegate, const FText&, Text);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWFTextFieldTextCommittedDelegate, const FText&, Text, ETextCommit::Type, CommitMethod);

UENUM(BlueprintType)
enum class ETextFilter : uint8
{
	None			UMETA(DisplayName="None"),
	TextOnly		UMETA(DisplayName="Text Only"),
	NumbersOnly		UMETA(DisplayName="Numbers Only"),

	TextFilter_Max	UMETA(Hidden),
};

UENUM(BlueprintType)
enum class ETextFieldIconVisibility : uint8
{
	Hidden					UMETA(DisplayName="Hidden"),
	Leading					UMETA(DisplayName="Leading"),
	Trailing				UMETA(DisplayName="Trailing"),
	LeadingAndTrailing		UMETA(DisplayName="Leading & Trailing"),

	TextFieldStyle_Max		UMETA(Hidden),
};

UENUM(BlueprintType)
enum class ETextFieldState : uint8
{
	Normal		UMETA(DisplayName="Normal"),
	Focused		UMETA(DisplayName="Focused"),
	Accepted	UMETA(DisplayName="Accepted"),
	Warning		UMETA(DisplayName="Warning"),
	Error		UMETA(DisplayName="Error"),

	TextFieldState_Max	UMETA(Hidden),
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WIDGETSTUDIORUNTIME_API UWidgetStudioModernTextField : public UWidgetStudioBase
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

	UFUNCTION()
	void OnTextInputTextChanged(const FText& InText);

	UFUNCTION()
	void OnTextInputTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod);

	UFUNCTION()
	EPalette GetColorByState() const;
	
	/* Widget Components */

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UInvalidationBox* Retainer = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UVerticalBox* VBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UTextBlock* LabelItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* SizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* Overlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* BackgroundScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* BackgroundOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* DropShadow = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* Background = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* BackgroundOutline = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UHorizontalBox* HBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* LeadingIconScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* LeadingIconItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* TrailingIconScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* TrailingIconItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UEditableTextBox* TextInput = nullptr;


private:

	/** The current state of the text field. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	ETextFieldState State = ETextFieldState::Normal;

	/** The label of the text field. If empty, the label will be hidden. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	FText Label;
	
	/** The text displayed in the Text Field when no text has been inputted. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	FText PlaceholderText = FText().FromString("Enter text here!");

	/** The text displayed in the Text Field. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	FText Text = FText().FromString("");

	/** The style of the corners. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	ECornerStyle CornerStyle = ECornerStyle::Rounded;

	/** How the text should be filtered. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	ETextFilter Filter = ETextFilter::None;

	/** The limit on how many characters should allowed. 0 = Uncapped */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (ClampMin = "0", UIMin = "0"))
	int32 CharacterLimit = 0;

	/** Set the Text Field to password mode. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	bool bIsPasswordField = false;


	/* Icons */

	/** Set the visibility of the leading icon. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Icon")
	ETextFieldIconVisibility IconVisibility = ETextFieldIconVisibility::Hidden;
	
	/** The icon displayed on the left side. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Icon")
	EIconItem LeadingIcon = EIconItem::LockClose;

	/** The icon displayed on the right side. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Icon")
	EIconItem TrailingIcon = EIconItem::Heart;

	/** Adjust the size of the icons. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Icon", Meta = (ClampMin = "12", UIMin = "12"))
	float IconSize = 16;
	
	
	/* Colors */

	/** The color of the background. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette BackgroundColor = EPalette::TertiaryBackground;

	/** The color of the outline when in a normal state. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette DefaultOutline = EPalette::Transparent;

	/** The color of the content (Text/Icon). */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette ContentColor = EPalette::PrimaryContent;
	
	/** The color of the label. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette LabelColor = EPalette::PrimaryContent;

public:

	/* Bindings */

	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWFTextFieldTextChangedDelegate OnTextChanged;

	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWFTextFieldTextCommittedDelegate OnTextCommitted;

	/* Helpers */

	/** Returns the label. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	FText GetLabel() const;

	/** Returns the text in the text field. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	FText GetText() const;

	/** Returns the placeholder text. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	FText GetPlaceholderText() const;

	/** Returns the current set text filter. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	ETextFilter GetFilter() const;

	/** Returns the character limit. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	int32 GetCharacterLimit() const;

	/** Returns the current icon visibility setting. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	ETextFieldIconVisibility GetIconVisibility() const;

	/** Returns the leading icon. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	EIconItem GetLeadingIcon() const;

	/** Returns the trailing icon. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	EIconItem GetTrailingIcon() const;

	/** Returns the current Text Field State. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	ETextFieldState GetState() const;

	/** Returns the background color. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	EPalette GetBackgroundColor() const;

	/** Returns the background color. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	EPalette GetContentColor() const;

	/** Returns the icon size */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	float GetIconSize() const;

	
	/* Modifiers */

	/** Sets the label that appears above the text field. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetLabel(FText NewLabel);

	/** Sets the text in the Text Field. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetText(FText NewText);

	/** Sets the placeholder text in the Text Field. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetPlaceholderText(FText NewText);

	/** Sets and applies a new filter for the text in the text field. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetFilter(ETextFilter NewFilter);

	/** Sets and applies the character limit. If set to 0, then no limit is imposed. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetCharacterLimit(int32 NewLimit);

	/** Sets the icon visibility. */
    UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
    void SetIconVisibility(ETextFieldIconVisibility NewState);

	/** Sets the leading icon. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetLeadingIcon(EIconItem NewIcon);

	/** Sets the trailing icon. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetTrailingIcon(EIconItem NewIcon);

	/** Manually set the state of the text field. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetState(ETextFieldState NewState);

	/** Set the background color. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetBackgroundColor(EPalette NewColor);

	/** Set the content color. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetContentColor(EPalette NewColor);

	/** Set the icon size. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetIconSize(float NewSize);
};
