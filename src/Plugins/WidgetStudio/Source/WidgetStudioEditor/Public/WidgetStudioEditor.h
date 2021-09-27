// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "WSSettings.h"

class FWidgetStudioEditor final : public IModuleInterface
{

public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override;

	UWidgetStudioSettings* WfSettings;

private:
	bool HandleSettingsSaved() const;
	void RegisterSettings() const;
	void UnregisterSettings() const;
	static TSharedPtr< class FSlateStyleSet > StyleSet;

};