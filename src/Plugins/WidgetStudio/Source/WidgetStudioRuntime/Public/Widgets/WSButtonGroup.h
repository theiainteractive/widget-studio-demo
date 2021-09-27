// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WSBase.h"
#include "WSButtonBase.h"
#include "WSModernCheckBox.h"
#include "WSButtonGroup.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWFButtonGroupChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWFButtonGroupDelegate, int32, Index);

/**
 * A utility widget to group buttons and their logic together.
 */
UCLASS(BlueprintType, Blueprintable)
class WIDGETSTUDIORUNTIME_API UWidgetStudioButtonGroup : public UWidgetStudioBase
{
	GENERATED_BODY()

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void SynchronizeProperties() override;;

protected:

	#if WITH_EDITOR
		virtual const FText GetPaletteCategory() override;
	#endif
	
	UFUNCTION()
	void OnButtonPressed(UWidgetStudioButtonBase* Button);

	UFUNCTION()
	void UpdateCurrentIndex();

	UFUNCTION()
	void UpdateButtonCheckedStates();
	

private:
	/* The button selection method.
	 * Inclusive: Multiple buttons can be selected at a time.
	 * Exclusive: Only a single button can be selected at a time.
	 */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	EClusivity SelectionMethod = EClusivity::Exclusive;

	// The index of the button currently active. If in Inclusive mode, this will be the last toggled button index.
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	int32 CurrentIndex = -1;

	// Add the buttons to be managed by this group.
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	TArray<UWidgetStudioButtonBase*> Buttons;

	// The buttons being managed by the group.
	UPROPERTY()
	TArray<UWidgetStudioButtonBase*> ManagedButtons = {};

	// The latest button pressed.
	UPROPERTY()
	UWidgetStudioButtonBase* CurrentButton = nullptr;

public:
	/* Bindings */

	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWFButtonGroupDelegate OnCurrentIndexChanged;

	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWFButtonGroupChangedDelegate OnButtonsChanged;

	
	/* Helpers */

	// Returns the index of the last button to be selected.
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	int32 GetCurrentIndex() const;

	// Returns all of the buttons managed by the Button Group
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	TArray<UWidgetStudioButtonBase*> GetButtons() const;

	// Returns the button at the given index.
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	UWidgetStudioButtonBase* GetButtonAtIndex(int32 Index);

	// Returns the button at the current index.
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	UWidgetStudioButtonBase* GetCurrentButton();

	// Returns the number of buttons contained within the Button Group.
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	int32 GetButtonCount() const;

	// Returns the current selection method.
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	EClusivity GetSelectionMethod() const;


	/* Modifiers */

	// Add a button to the Button Group
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void AddButton(UWidgetStudioButtonBase* NewButton);

	// Remove a button from the Button Group
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	bool RemoveButton(UWidgetStudioButtonBase* ButtonToRemove);

	// Remove button at given index.
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void RemoveButtonAtIndex(int32 Index);

	// Removes all buttons from Button Group.
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void RemoveAllButtons();

	// Set the currently selected button.
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetCurrentIndex(int32 Index);

	// Set the currently selected button.
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetSelectionMethod(EClusivity NewMethod);

};
