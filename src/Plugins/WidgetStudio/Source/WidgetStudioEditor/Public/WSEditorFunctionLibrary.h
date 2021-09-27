// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetBlueprint.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WSEditorFunctionLibrary.generated.h"

/**
 * A Widget Studio library for Editor only functions. Do _not_ attempt these for gameplay.
 */
UCLASS()
class WIDGETSTUDIOEDITOR_API UWidgetStudioEditorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	/** Opens an Editor Utility Widget in a new Editor window. If the window already exists, it will bring it into focus.
	 *
	 * NOTE: Only works with Editor Utility Widgets and will NOT work with regular Widget Blueprints.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Function Library|Editor")
	static void OpenEditorUtilityWidget(UWidgetBlueprint* WidgetBlueprint);
};
