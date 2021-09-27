// Fill out your copyright notice in the Description page of Project Settings.


#include "WSSettings.h"
#include "FileHelpers.h"
#include "WSSubsystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Subsystems/AssetEditorSubsystem.h"

UWidgetStudioSettings::UWidgetStudioSettings()
{
	UpdateThemePreview();
}

void UWidgetStudioSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UWidgetStudioSubsystem* WidgetStudioSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UWidgetStudioSettings, CreateThemeButton))
	{
		CreateNewThemeDataAssetObject();
		CreateThemeButton = false;
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UWidgetStudioSettings, CreateTypographyButton))
	{
		CreateNewTypographyDataAssetObject();
		CreateTypographyButton = false;
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UWidgetStudioSettings, CreateIconSetButton))
	{
		CreateNewIconSetDataAssetObject();
		CreateIconSetButton = false;
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UWidgetStudioSettings, Theme))
	{
		UpdateThemePreview();
		WidgetStudioSubsystem->SetTheme(Theme);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UWidgetStudioSettings, Typography))
	{
		WidgetStudioSubsystem->SetTypography(Typography);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UWidgetStudioSettings, IconSet))
	{
		WidgetStudioSubsystem->SetIconSet(IconSet);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UWidgetStudioSettings, BorderRadius))
	{
		WidgetStudioSubsystem->SetBorderRadius(BorderRadius);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UWidgetStudioSettings, IdealWidth) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(UWidgetStudioSettings, IdealHeight)
		)
	{
		const FVector2D ControlDimensions(IdealWidth, IdealHeight);
		WidgetStudioSubsystem->SetControlDimensions(ControlDimensions);
	}

	SaveConfig();
}

UWidgetStudioSettings::UWidgetStudioSettings (const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UWidgetStudioSettings::UpdateThemePreview()
{
	if (Theme.IsValid())
	{
			PrimaryAccent			= Theme->PrimaryAccent;
			SecondaryAccent			= Theme->SecondaryAccent;
			PrimaryContent			= Theme->PrimaryContent;
			SecondaryContent		= Theme->SecondaryContent;
			TertiaryContent			= Theme->TertiaryContent;
			PrimaryBackground		= Theme->PrimaryBackground;
			SecondaryBackground 	= Theme->SecondaryBackground;
			TertiaryBackground		= Theme->TertiaryBackground;
	}
}

// TODO Convert to abstract creation function
void UWidgetStudioSettings::CreateNewThemeDataAssetObject() const
{
	const FString FileName = TEXT("NewTheme");
	const FString PackageName = TEXT("/Game/WidgetStudio/Themes/NewTheme");
	UPackage* Package = CreatePackage(*PackageName);

	// Construct new data asset object
	UWidgetStudioTheme* NewDataAssetObject = NewObject<UWidgetStudioTheme>(Package, UWidgetStudioTheme::StaticClass(), *FileName, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);
	FAssetRegistryModule::AssetCreated(NewDataAssetObject);
	

	// Open data asset object property window
	if (GEditor)
	{
		UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
		AssetEditorSubsystem->OpenEditorForAsset(NewDataAssetObject);
	}

	// Save new data asset object
	Package->SetDirtyFlag(true);
	TArray<UPackage*> PackagesToSave;
	PackagesToSave.Add(Package);
	FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, false, false);

	ProcessNewlyLoadedUObjects();
}

void UWidgetStudioSettings::CreateNewTypographyDataAssetObject() const
{
	const FString FileName = TEXT("NewTypography");
	const FString PackageName = TEXT("/Game/WidgetStudio/Typography/NewTypography");
	UPackage* Package = CreatePackage(*PackageName);

	// Construct new data asset object
	UWidgetStudioTypography* NewDataAssetObject = NewObject<UWidgetStudioTypography>(Package, UWidgetStudioTypography::StaticClass(), *FileName, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);
	FAssetRegistryModule::AssetCreated(NewDataAssetObject);
	

	// Open data asset object property window
	if (GEditor)
	{
		UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
		AssetEditorSubsystem->OpenEditorForAsset(NewDataAssetObject);
	}

	// Save new data asset object
	Package->SetDirtyFlag(true);
	TArray<UPackage*> PackagesToSave;
	PackagesToSave.Add(Package);
	FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, false, false);

	ProcessNewlyLoadedUObjects();
}

void UWidgetStudioSettings::CreateNewIconSetDataAssetObject() const
{
	const FString FileName = TEXT("NewIconSet");
	const FString PackageName = TEXT("/Game/WidgetStudio/IconSets/NewIconSet");
	UPackage* Package = CreatePackage(*PackageName);

	// Construct new data asset object
	UWidgetStudioIconSet* NewDataAssetObject = NewObject<UWidgetStudioIconSet>(Package, UWidgetStudioIconSet::StaticClass(), *FileName, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);
	FAssetRegistryModule::AssetCreated(NewDataAssetObject);
	

	// Open data asset object property window
	if (GEditor)
	{
		UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
		AssetEditorSubsystem->OpenEditorForAsset(NewDataAssetObject);
	}

	// Save new data asset object
	Package->SetDirtyFlag(true);
	TArray<UPackage*> PackagesToSave;
	PackagesToSave.Add(Package);
	FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, false, false);

	ProcessNewlyLoadedUObjects();
}
