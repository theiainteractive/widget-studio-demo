// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WSBase.h"
#include "WSModernButton.h"
#include "WSContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWFContainer, int32, CurrentIndex, FButtonOptions, Option);

/**
 * 
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioContainer : public UWidgetStudioBase
{
	GENERATED_BODY()

protected:
	virtual void ConstructOption(FButtonOptions Option);
	virtual void SynchronizeProperties() override;

private:
	// The current option selected. A value of -1 will indicate that no option is selected.
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (MinClamp = "-1"))
	int32 CurrentIndex = -1;
	
	// The options that will be available.
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (TitleProperty = "Text"))
	TArray<FButtonOptions> Options;

public:
	// Bindings
	
	/** Called when the current index has been changed. */
	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWFContainer OnCurrentIndexChanged;

	// Helpers

	/** Return the index of the current selected button. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	int32 GetCurrentIndex() const;

	/** Returns the number of options */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	int32 GetOptionCount() const;

	/** Returns the options. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	TArray<FButtonOptions> GetOptions() const;

	/**Return the option at the given index. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	FButtonOptions GetOptionAtIndex(int32 Index);

	/** Get the options of the current button. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	FButtonOptions GetCurrentOption() const;

	/** Returns the Option Index based on given text. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Helper")
	int32 GetOptionIndexViaText(FText InText);

	/** Increases the current index by 1.*/
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Helper")
	void IncrementCurrentIndex();

	/** Decreases the current index by 1. Can only go as low as index 0.*/
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Helper")
	void DecrementCurrentIndex();

	/** Sets the current index to -1. No option will be labeled as selected.*/
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Helper")
	void ClearCurrentIndex();


	// Modifiers

	/** Override the options in this container. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	virtual void SetOptions(TArray<FButtonOptions> NewOptions);

	/** Set the current index. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	virtual void SetCurrentIndex(int32 Index);

	/** Add an option to the container. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void AddOption(FButtonOptions NewOption);

	/** Clear all options from the container. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	virtual void ClearOptions();

	/** Finds and sets the current index based on option text. Returns false if it could not find the option. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	bool SetCurrentIndexViaOptionText(FText InText);
};

