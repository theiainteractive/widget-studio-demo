// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/* Do Not Remove */
#include "Modules/ModuleManager.h"

#include "Materials/Material.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWidgetStudio, All, All);

class FWidgetStudioRuntime : public IModuleInterface
{

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};


// Common Structs

// Templates
template <typename ObjClass>
static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path)
{
	if (Path == NAME_None) return nullptr;

	return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), nullptr, *Path.ToString()));
}

static FORCEINLINE UMaterial* LoadMaterialFromPath(const FName& Path)
{
	if (Path == NAME_None) return nullptr;

	return LoadObjFromPath<UMaterial>(Path);
}


