// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WSBase.h"
#include "Blueprint/UserWidget.h"
#include "WSButtonBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWFButtonDelegate, UWidgetStudioButtonBase*, CallingButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FButtonToggledDelegate, UWidgetStudioButtonBase*, CallingButton, bool, NewCheckedState);

/**
 * Contain the fundamental logic for a Widget Studio button.
 * Subclass to create custom buttons that are compatible with Widget Studio.
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioButtonBase : public UWidgetStudioBase
{
	GENERATED_BODY()

	// Depicts if the button is currently being pressed down.
	bool bIsPressed = false;

protected:
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	FTimerHandle DoublePressTimer;

private:	
	/* States */

	// Depicts if the button can be checked or not.
	UPROPERTY(EditAnywhere, Category = "Widget Studio|State")
	bool bIsCheckable = false;

	// Depicts if the button is checked (on/true) or not (off/false). Requires the bIsCheckable state to be enabled to utilize.
	UPROPERTY(EditAnywhere, Category = "Widget Studio|State")
	bool bIsChecked = false;

public:

	/* Bindings */

	// Called when the button is pressed with the left mouse button.
	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWFButtonDelegate OnPressed;

	// Called when the button is released.
	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
    FWFButtonDelegate OnReleased;

	// Called when the button is quickly pressed twice with the left mouse button.
	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWFButtonDelegate OnDoublePressed;

	// Called when the buttons checked status is toggled. Only called when the button is set to checkable.
	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FButtonToggledDelegate OnToggled;

	UPROPERTY()
	bool bForceCheckable = false;

	// Internal. Used to make the button non-interactable when checked.
	UPROPERTY()
	bool bIsInteractable = true;

	/* Helpers */

	// Returns the checkable state of the button.
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|State")
	bool IsCheckable() const;

	// Returns the checked state of the button. If bIsCheckable is false, this will always return false.
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|State")
	bool IsChecked() const;

	// Returns the pressed state of the button.
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|State")
	bool IsPressed() const;

	/* Modifiers */

	// Set the checkable state of the button.
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|State")
	virtual void SetCheckable(bool bNewCheckableState);

	// Set the checked state of the button. Does nothing if the button isn't checkable.
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|State")
	virtual void SetChecked(bool bNewCheckedState);
};
