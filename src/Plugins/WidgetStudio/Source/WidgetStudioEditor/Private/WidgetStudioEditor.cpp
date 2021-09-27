// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "WidgetStudioEditor.h"
#include "WSSubsystem.h"
#include "WSSettings.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"
#include "WSGlobals.h"

#define LOCTEXT_NAMESPACE "FWidgetStudioEditor"
DEFINE_LOG_CATEGORY(LogWidgetStudio);


void FWidgetStudioEditor::StartupModule()
{
	RegisterSettings();
	WfSettings = GetMutableDefault<UWidgetStudioSettings>();

	WfSettings->Theme = DefaultTheme;
	WfSettings->IconSet = DefaultIconSet;
	WfSettings->Typography = DefaultTypography;

	//if there are values in ini file, override defaults
	WfSettings->LoadConfig();

	//initial loading of assets
	LoadObject<UWidgetStudioTheme>(nullptr, *WfSettings->Theme.ToString());
	LoadObject<UWidgetStudioIconSet>(nullptr, *WfSettings->IconSet.ToString());
	LoadObject<UWidgetStudioTypography>(nullptr, *WfSettings->Typography.ToString());

	//save values to ini file in case any of them are missing
	WfSettings->SaveConfig();
}

void FWidgetStudioEditor::ShutdownModule()
{
	if (UObjectInitialized())
	{
		UnregisterSettings();
	}
}

bool FWidgetStudioEditor::SupportsDynamicReloading()
{
	return true;
}

bool FWidgetStudioEditor::HandleSettingsSaved() const
{
	UWidgetStudioSettings* Settings = GetMutableDefault<UWidgetStudioSettings>();
	constexpr bool ReSaveSettings = false; // FIXME: This bool is always false, thus Settings->SaveConfig() is never used.

	if (ReSaveSettings)
	{
		Settings->SaveConfig();
	}

	return true;
}

void FWidgetStudioEditor::RegisterSettings() const
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Create a new category
		const ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");

		SettingsContainer->DescribeCategory(
			"Theia Interactive",
			LOCTEXT("RuntimeWDCategoryName", "Theia Interactive"),
			LOCTEXT("RuntimeWDCategoryDescription", "Game configuration settings for plugins created by Theia Interactive.")
			);

		// Register Settings
		const ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings(
			"Project",
			"Theia Interactive",
			"Widget Studio",
			LOCTEXT("RuntimeGeneralSettingsName", "Widget Studio"),
			LOCTEXT("RuntimeGeneralSettingsDescription", "Configure the properties of the Widget Studio."),
			GetMutableDefault<UWidgetStudioSettings>()
			);

		// Register the save handler
		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FWidgetStudioEditor::HandleSettingsSaved);
		}
	}
}

void FWidgetStudioEditor::UnregisterSettings() const
{
	// Ensure to unregister all of your registered settings here, hot-reload would
	// otherwise yield unexpected results.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Theia Interactive", "General");
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FWidgetStudioEditor, WidgetStudioEditor)
