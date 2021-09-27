#include "WSSubsystem.h"
#include "Misc/Paths.h"
#include "Misc/ConfigCacheIni.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Modules/ModuleManager.h"
#include "WSGlobals.h"
#include "Engine/Engine.h"

UWidgetStudioSubsystem *UWidgetStudioSubsystem::Instance = nullptr;


/**
* constructor -- initialize with default values in case values are not contained in config file.  Then load values from config file (replaces defaults).
* Then save values to config file in case any did not exist there previously.
*/
UWidgetStudioSubsystem::UWidgetStudioSubsystem()
{
	OnPluginStartedInit.Broadcast();

	// bind to OnAssetUpdated so that, if a data asset is moved, the path of the object (i.e. UWidgetStudioSubsystem::Theme),
	// can be updated, even if no redirectors exist
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();
	AssetRegistry.OnAssetRenamed().AddUObject(this, &UWidgetStudioSubsystem::HandleAssetRenamed);
	AssetRegistry.OnAssetAdded().AddUObject(this, &UWidgetStudioSubsystem::HandleAssetAdded);

	//set defaults
	Theme = DefaultTheme;
	IconSet = DefaultIconSet;
	Typography = DefaultTypography;

	//if there are values in ini file, override defaults
	LoadValuesFromIni();

	//initial loading of assets
	LoadObject<UWidgetStudioTheme>(nullptr, *Theme.ToString());
	if (!IsValid(Theme.Get()))
	{
		const TSoftObjectPtr<UWidgetStudioTheme> DefaultThemePtr(DefaultTheme);
		SetTheme(DefaultThemePtr);
	}

	LoadObject<UWidgetStudioIconSet>(nullptr, *IconSet.ToString());
	if (!IsValid(IconSet.Get()))
	{
		const TSoftObjectPtr<UWidgetStudioIconSet> DefaultIconSetPtr(DefaultIconSet);
		SetIconSet(DefaultIconSetPtr);
	}

	LoadObject<UWidgetStudioTypography>(nullptr, *Typography.ToString());
	if (!IsValid(Typography.Get()))
	{
		const TSoftObjectPtr<UWidgetStudioTypography> DefaultTypographyPtr(DefaultTypography);
		SetTypography(DefaultTypographyPtr);
	}

	OnStyleChanged.Broadcast();

	//save values to ini file in case any of them are missing
	SaveValuesToIni();
}

/**
* destructor
*/
UWidgetStudioSubsystem::~UWidgetStudioSubsystem()
{
}

/**
* Get WidgetStudioSingleton instance.  Create if non-existent.
* @return the instance
*/
UWidgetStudioSubsystem *UWidgetStudioSubsystem::GetInstance()
{
	if (!Instance)
		Instance = NewObject<UWidgetStudioSubsystem>(UWidgetStudioSubsystem::StaticClass());
	return Instance;
}

/**
* Make sure current Theme DataAsset is loaded and valid, then retrieve it
* @return the UWidgetStudioTheme*
*/
UWidgetStudioTheme *UWidgetStudioSubsystem::GetTheme()
{
	if (!GConfig)
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not get GConfig."));
		return nullptr;
	}

	UWidgetStudioTheme* ReturnTheme = nullptr;
	const TCHAR* Section = TEXT("/Script/WidgetStudioEditor.WidgetStudioSettings");
	const TCHAR* Key = TEXT("Theme");
	FString ThemeValue;
	const FString IniFile = FPaths::ProjectConfigDir() + TEXT("/DefaultGame.ini");

	if (GConfig->GetString(Section, Key, ThemeValue, IniFile))
	{
		ReturnTheme = LoadObject<UWidgetStudioTheme>(nullptr, *ThemeValue);
		if (IsValid(ReturnTheme))
		{
			return ReturnTheme;
		}
	}

	// if ini theme is invalid or we couldn't read from ini, set default theme
	UE_LOG(LogWidgetStudio, Warning, TEXT("Current theme not valid.  Setting default theme."));
	ThemeValue = DefaultTheme;
	const FString Default = DefaultTheme;
	Theme = Default;
	if (SetTheme(Theme))
	{
		ReturnTheme = LoadObject<UWidgetStudioTheme>(nullptr, *ThemeValue);
	}
	else
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not set default theme."));
	}

	if (!IsValid(ReturnTheme))
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not get theme."));	
	}

	return ReturnTheme;
}

/**
* Make sure current IconSet DataAsset is loaded and valid, then retrieve it
* @return the UWidgetStudioIconSet*
*/
UWidgetStudioIconSet *UWidgetStudioSubsystem::GetIconSet()
{
	if (!GConfig)
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not get GConfig."));
		return nullptr;
	}

	UWidgetStudioIconSet* ReturnIconSet = nullptr;
	const TCHAR* Section = TEXT("/Script/WidgetStudioEditor.WidgetStudioSettings");
	const TCHAR* Key = TEXT("IconSet");
	FString IconSetValue;
	const FString IniFile = FPaths::ProjectConfigDir() + TEXT("/DefaultGame.ini");

	if (GConfig->GetString(Section, Key, IconSetValue, IniFile))
	{
		ReturnIconSet = LoadObject<UWidgetStudioIconSet>(nullptr, *IconSetValue);
		if (IsValid(ReturnIconSet))
		{
			return ReturnIconSet;
		}
	}

	// if ini icon set is invalid or we couldn't read from ini, set default icon set
	UE_LOG(LogWidgetStudio, Warning, TEXT("Current icon set not valid.  Setting default icon set."));
	IconSetValue = DefaultIconSet;
	const FString Default = DefaultIconSet;
	IconSet = Default;
	if (SetIconSet(IconSet))
	{
		ReturnIconSet = LoadObject<UWidgetStudioIconSet>(nullptr, *IconSetValue);
	}
	else
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not set default icon set."));
	}

	if (!IsValid(ReturnIconSet))
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not get icon set."));	
	}

	return ReturnIconSet;
}

/**
* Make sure current Typography DataAsset is loaded and valid, then retrieve it
* @return the UWidgetStudioTypography*
*/
UWidgetStudioTypography *UWidgetStudioSubsystem::GetTypography()
{
	if (!GConfig)
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not get GConfig."));
		return nullptr;
	}

	UWidgetStudioTypography* ReturnTypography = nullptr;
	const TCHAR* Section = TEXT("/Script/WidgetStudioEditor.WidgetStudioSettings");
	const TCHAR* Key = TEXT("Typography");
	FString TypographyValue;
	const FString IniFile = FPaths::ProjectConfigDir() + TEXT("/DefaultGame.ini");

	if (GConfig->GetString(Section, Key, TypographyValue, IniFile))
	{
		ReturnTypography = LoadObject<UWidgetStudioTypography>(nullptr, *TypographyValue);
		if (IsValid(ReturnTypography))
		{
			return ReturnTypography;
		}
	}

	// if ini typography is invalid or we couldn't read from ini, set default typography
	UE_LOG(LogWidgetStudio, Warning, TEXT("Current typography not valid.  Setting default typography."));
	TypographyValue = DefaultTypography;
	const FString Default = DefaultTypography;
	Typography = Default;
	if (SetTypography(Typography))
	{
		ReturnTypography = LoadObject<UWidgetStudioTypography>(nullptr, *TypographyValue);
	}
	else
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not set default typography."));
	}

	if (!IsValid(ReturnTypography))
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not get typography."));	
	}

	return ReturnTypography;
}

/**
* Get current border radius value
* @return BorderRadius
*/
int32 UWidgetStudioSubsystem::GetBorderRadius()
{
	const TCHAR *Section = TEXT("/Script/WidgetStudioEditor.WidgetStudioSettings");
	const TCHAR *Key = TEXT("BorderRadius");
	int32 Radius;
	const FString IniFile = FPaths::ProjectConfigDir() + TEXT("/DefaultGame.ini");

	if (!GConfig->GetInt(Section, Key, Radius, IniFile))
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not read border radius from .ini."));
	}

	return Radius;
}

/**
* Get current control dimensions x and y values as FVector2D
* @return ControlDimensions
*/
FVector2D UWidgetStudioSubsystem::GetControlDimensions()
{
	FVector2D ControlDimensions;
	if (GConfig)
	{
		const TCHAR *KeyWidth = TEXT("IdealWidth");
		const TCHAR *KeyHeight = TEXT("IdealHeight");
		const TCHAR *Section = TEXT("/Script/WidgetStudioEditor.WidgetStudioSettings");
		float DimX, DimY;
		const FString IniFile = FPaths::ProjectConfigDir() + TEXT("/DefaultGame.ini");

		if (!GConfig->GetFloat(Section, KeyWidth, DimX, IniFile))
		{
			UE_LOG(LogWidgetStudio, Warning, TEXT("Could not read IdealWidth from .ini."));
		}
		ControlDimensions.X = DimX;

		if (!GConfig->GetFloat(Section, KeyHeight, DimY, IniFile))
		{
			UE_LOG(LogWidgetStudio, Warning, TEXT("Could not read IdealHeight from .ini."));
		}
		ControlDimensions.Y = DimY;
	}
	return ControlDimensions;
}

/**
* Sets the theme (colors) for widgets -- checks to make sure the incoming Theme is valid and loaded, then updates
* the current Theme, including the "Theme" value in the .ini file. Broadcasts OnStyleChanged() event.
* @param InTheme - the Theme to set
* @return true if Theme is successfully set, false otherwise 
*/
bool UWidgetStudioSubsystem::SetTheme(const TSoftObjectPtr<UWidgetStudioTheme> InTheme)
{
	LoadObject<UWidgetStudioTheme>(nullptr, *InTheme.ToString());
	if (InTheme.IsValid())
	{
		const TSoftObjectPtr<UWidgetStudioTheme> TempTheme = Theme;
		Theme = InTheme;
		LoadObject<UWidgetStudioTheme>(nullptr, *Theme.ToString());
		if (Theme.IsValid())
		{
			GConfig->SetString(*WSDefaultGameSection, *StringTheme, *Theme.ToString(), DefaultGameIni);
			GConfig->Flush(false, DefaultGameIni);
			OnStyleChanged.Broadcast();
			return true;
		}
		else
		{
			Theme = TempTheme;
			UE_LOG(LogWidgetStudio, Warning, TEXT("Could not load Theme asset"));
			return false;
		}
	}

	UE_LOG(LogWidgetStudio, Warning, TEXT("Theme %s is invalid"), *GetNameSafe(InTheme.Get()));
	return false;
}

/**
* Sets the IconStyle for widgets -- checks to make sure the incoming IconSet is valid and loaded, then updates
* the current IconSet, including the "IconSet" value in the .ini file. Broadcasts OnStyleChanged() event.
* @param InIconSet - the IconSet to set
* @return true if IconSet is successfully set, false otherwise
*/
bool UWidgetStudioSubsystem::SetIconSet(const TSoftObjectPtr<UWidgetStudioIconSet> InIconSet)
{
	LoadObject<UWidgetStudioIconSet>(nullptr, *InIconSet.ToString());

	if (InIconSet.IsValid())
	{
		const TSoftObjectPtr<UWidgetStudioIconSet> TempIconSet = IconSet;
		IconSet = InIconSet;
		LoadObject<UWidgetStudioIconSet>(nullptr, *IconSet.ToString());

		if (IconSet.IsValid())
		{
			GConfig->SetString(*WSDefaultGameSection, *StringIconSet, *IconSet.ToString(), DefaultGameIni);
			GConfig->Flush(false, DefaultGameIni);
			OnStyleChanged.Broadcast();
			return true;
		}
		else
		{
			IconSet = TempIconSet;
			UE_LOG(LogWidgetStudio, Warning, TEXT("Could not load IconSet asset"));
			return false;
		}
	}

	UE_LOG(LogWidgetStudio, Warning, TEXT("IconSet %s is invalid"), *GetNameSafe(InIconSet.Get()));
	return false;
}

/**
* Sets the typography (font settings) for widgets -- checks to make sure the incoming Typography is valid and loaded, then updates
* the current Typography, including the "Typography" value in the .ini file. Broadcasts OnStyleChanged() event.
* @param InTypography - the Typography to set
* @return true if Typography is successfully set, false otherwise
*/
bool UWidgetStudioSubsystem::SetTypography(const TSoftObjectPtr<UWidgetStudioTypography> InTypography)
{
	LoadObject<UWidgetStudioTypography>(nullptr, *InTypography.ToString());

	if (InTypography.IsValid())
	{
		const TSoftObjectPtr<UWidgetStudioTypography> TempTypography = Typography;
		Typography = InTypography;
		LoadObject<UWidgetStudioTypography>(nullptr, *Typography.ToString());

		if (Typography.IsValid())
		{
			GConfig->SetString(*WSDefaultGameSection, *StringTypography, *Typography.ToString(), DefaultGameIni);
			GConfig->Flush(false, DefaultGameIni);
			OnStyleChanged.Broadcast();
			return true;
		}
		else
		{
			Typography = TempTypography;
			UE_LOG(LogWidgetStudio, Warning, TEXT("Could not load Typography asset"));
			return false;
		}
	}

	UE_LOG(LogWidgetStudio, Warning, TEXT("Typography %s is invalid"), *GetNameSafe(InTypography.Get()));
	return false;
}

/**
* Sets the border radius for widgets -- updates BorderRadius, oncluding "BorderRadius" value in the .ini file.
* Broadcasts OnStyleChanged() event.
* @param InRadius
* @return true if border radius is successfully set, false otherwise
*/
bool UWidgetStudioSubsystem::SetBorderRadius(int32 InRadius)
{
	BorderRadius = InRadius;
	GConfig->SetInt(*WSDefaultGameSection, *StringBorderRadius, BorderRadius, DefaultGameIni);
	GConfig->Flush(false, DefaultGameIni);
	OnStyleChanged.Broadcast();
	return true;
	//should return false if write fails
}

/**
* Sets the control dimensions for widgets -- updates ControlDimensions, oncluding "ControlDimensions" value in the .ini file.
* Broadcasts OnStyleChanged() event.
* @param InDimensions
* @return true if control dimensions are successfully set, false otherwise
*/
bool UWidgetStudioSubsystem::SetControlDimensions(const FVector2D InDimensions)
{
	IdealWidth = InDimensions.X;
	IdealHeight = InDimensions.Y;
	GConfig->SetFloat(*WSDefaultGameSection, *StringIdealWidth, IdealWidth, DefaultGameIni);
	GConfig->SetFloat(*WSDefaultGameSection, *StringIdealHeight, IdealHeight, DefaultGameIni);
	GConfig->Flush(false, DefaultGameIni);
	OnStyleChanged.Broadcast();
	return true;
	//should return false if write fails
}

void UWidgetStudioSubsystem::LoadValuesFromIni()
{
	FString ReadString;

	if (GConfig->GetString(*WSDefaultGameSection, *StringTheme, ReadString, DefaultGameIni))
	{
		Theme = ReadString;
	}
	if (GConfig->GetString(*WSDefaultGameSection, *StringIconSet, ReadString, DefaultGameIni))
	{
		IconSet = ReadString;
	}
	if (GConfig->GetString(*WSDefaultGameSection, *StringTypography, ReadString, DefaultGameIni))
	{
		Typography = ReadString;
	}
	GConfig->GetInt(*WSDefaultGameSection, *StringBorderRadius, BorderRadius, DefaultGameIni);
	GConfig->GetFloat(*WSDefaultGameSection, *StringIdealWidth, IdealWidth, DefaultGameIni);
	GConfig->GetFloat(*WSDefaultGameSection, *StringIdealHeight, IdealHeight, DefaultGameIni);
}

void UWidgetStudioSubsystem::SaveValuesToIni() const
{
	GConfig->SetString(*WSDefaultGameSection, *StringTheme, *Theme.ToString(), DefaultGameIni);
	GConfig->SetString(*WSDefaultGameSection, *StringIconSet, *IconSet.ToString(), DefaultGameIni);
	GConfig->SetString(*WSDefaultGameSection, *StringTypography, *Typography.ToString(), DefaultGameIni);
	GConfig->SetInt(*WSDefaultGameSection, *StringBorderRadius, BorderRadius, DefaultGameIni);
	GConfig->SetFloat(*WSDefaultGameSection, *StringIdealWidth, IdealWidth, DefaultGameIni);
	GConfig->SetFloat(*WSDefaultGameSection, *StringIdealHeight, IdealHeight, DefaultGameIni);
}

/**
* Keeps Theme, IconSet, Typography valid if data assets are renamed
* @param AssetData Asset data which includes the asset with the new name
* @param OldName The name before the change
*/
void UWidgetStudioSubsystem::HandleAssetRenamed(const FAssetData& AssetData, const FString& OldName)
{
	if (!GEngine) return;

	UWidgetStudioSubsystem* WidgetStudioSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	const auto Asset = AssetData.GetAsset();
	const FString ChangedName = AssetData.ObjectPath.ToString();

	if (Asset->GetClass() == UWidgetStudioTheme::StaticClass() || Asset->GetClass()->IsChildOf(UWidgetStudioTheme::StaticClass()))
	{
		if (WidgetStudioSubsystem->Theme->GetPathName() == ChangedName || WidgetStudioSubsystem->Theme->GetPathName() == OldName)
		{
			ChangedDataAssetName = ChangedName;
			ChangedDataAssetType = StringTheme;
			bIsDataAssetRenameInProcess = true;
		}
	}
	if (Asset->GetClass() == UWidgetStudioIconSet::StaticClass() || Asset->GetClass()->IsChildOf(UWidgetStudioIconSet::StaticClass()))
	{
		if (WidgetStudioSubsystem->IconSet->GetPathName() == ChangedName || WidgetStudioSubsystem->IconSet->GetPathName() == OldName)
		{
			ChangedDataAssetName = ChangedName;
			ChangedDataAssetType = StringIconSet;
			bIsDataAssetRenameInProcess = true;
		}
	}
	if (Asset->GetClass() == UWidgetStudioTypography::StaticClass() || Asset->GetClass()->IsChildOf(UWidgetStudioTypography::StaticClass()))
	{
		if (WidgetStudioSubsystem->Typography->GetPathName() == ChangedName || WidgetStudioSubsystem->Typography->GetPathName() == OldName)
		{
			ChangedDataAssetName = ChangedName;
			ChangedDataAssetType = StringTypography;
			bIsDataAssetRenameInProcess = true;
		}
	}
}

void UWidgetStudioSubsystem::HandleAssetAdded(const FAssetData& AssetData)
{
	if (!GEngine) return;

	UWidgetStudioSubsystem* WidgetStudioSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	if (bIsDataAssetRenameInProcess && AssetData.GetAsset()->GetPathName() == ChangedDataAssetName)
	{
		if (AssetData.GetAsset()->GetClass() == UWidgetStudioTheme::StaticClass() || AssetData.GetAsset()->GetClass()->IsChildOf(UWidgetStudioTheme::StaticClass()))
		{
			TSoftObjectPtr<UWidgetStudioTheme> ChangedAssetPtr(ChangedDataAssetName);
			SetTheme(ChangedAssetPtr);

			if (IsValid(WidgetStudioSubsystem->Theme.Get()))
			{
				bIsDataAssetRenameInProcess = false;
			}
		}

		if (AssetData.GetAsset()->GetClass() == UWidgetStudioIconSet::StaticClass() || AssetData.GetAsset()->GetClass()->IsChildOf(UWidgetStudioIconSet::StaticClass()))
		{
			TSoftObjectPtr<UWidgetStudioIconSet> ChangedAssetPtr(ChangedDataAssetName);
			SetIconSet(ChangedAssetPtr);

			if (IsValid(WidgetStudioSubsystem->IconSet.Get()))
			{
				bIsDataAssetRenameInProcess = false;
			}
		}

		if (AssetData.GetAsset()->GetClass() == UWidgetStudioTypography::StaticClass() || AssetData.GetAsset()->GetClass()->IsChildOf(UWidgetStudioTypography::StaticClass()))
		{
			TSoftObjectPtr<UWidgetStudioTypography> ChangedAssetPtr(ChangedDataAssetName);
			SetTypography(ChangedAssetPtr);

			if (IsValid(WidgetStudioSubsystem->Typography.Get()))
			{
				bIsDataAssetRenameInProcess = false;
			}
		}
	}
}

/**
* Engine Subsystem initializer
*/
void UWidgetStudioSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
	//Super::Initialize(Collection);

	bIsPluginInitialized = true;
	OnPluginFinishedInit.Broadcast();
}

/**
* Engine Subsystem Deinitializer
*/
void UWidgetStudioSubsystem::Deinitialize()
{
}
