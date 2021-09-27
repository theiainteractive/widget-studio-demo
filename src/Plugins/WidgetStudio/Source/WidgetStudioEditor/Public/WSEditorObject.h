// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WSEditorObject.generated.h"


/**
 *
 */
UCLASS()
class WIDGETSTUDIOEDITOR_API UWSEditorObject : public UObject
{
	GENERATED_BODY()

public:
	UWSEditorObject(const FObjectInitializer& ObjectInitializer);
	UWSEditorObject();

	UFUNCTION()
	void HandleStyleChanged();
};
