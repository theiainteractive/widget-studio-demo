#include "Tests/WSTestClass.h"
#include "Engine/Engine.h"
#include "Misc/ConfigCacheIni.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LogWidgetStudioTest);

UWidgetStudioTestClass::UWidgetStudioTestClass()
{
	const FString NordDark = TEXT("/WidgetStudio/Style/Themes/Theme_Nord_Dark.Theme_Nord_Dark");
	const FSoftObjectPath NordDarkRef = UKismetSystemLibrary::MakeSoftObjectPath(NordDark);
	NordDarkTheme = NordDarkRef.TryLoad();
	if (!IsValid(NordDarkTheme.Get()))
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Could not load asset: %s"), *NordDarkRef.ToString());
	}

	const FString NordLight = TEXT("/WidgetStudio/Style/Themes/Theme_Nord_Light.Theme_Nord_Light");
	const FSoftObjectPath NordLightRef = UKismetSystemLibrary::MakeSoftObjectPath(NordLight);
	NordLightTheme = NordLightRef.TryLoad();
	if (!IsValid(NordLightTheme.Get()))
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Could not load asset: %s"), *NordLightRef.ToString());
	}

	const FString DefaultIconSetString = TEXT("/WidgetStudio/Style/IconSets/IconSet_Default.IconSet_Default");
	const FSoftObjectPath DefaultRef = UKismetSystemLibrary::MakeSoftObjectPath(DefaultIconSetString);
	DefaultIconSet = DefaultRef.TryLoad();
	if (!IsValid(DefaultIconSet.Get()))
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Could not load asset: %s"), *DefaultRef.ToString());
	}

	const FString OpenSans = TEXT("/WidgetStudio/Style/Typography/Typography_OpenSans.Typography_OpenSans");
	const FSoftObjectPath OpenSansRef = UKismetSystemLibrary::MakeSoftObjectPath(OpenSans);
	OpenSansTypography = OpenSansRef.TryLoad();
	if (!IsValid(OpenSansTypography.Get()))
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Could not load asset: %s"), *OpenSansRef.ToString());
	}
}

UWidgetStudioTestClass::~UWidgetStudioTestClass()
{
}

/* Helper function -- Reset all values in the given .ini file to original defaults
* @param IniFile
*/

void UWidgetStudioTestClass::ResetTheme()
{
	if (!GEngine)
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't get GEngine."));
		return;
	}
	WSSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();

	WSSubsystem->Theme = NordDarkTheme;
	WSSubsystem->Theme.LoadSynchronous();
	if (!WSSubsystem->Theme.IsValid())
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't load Theme."));
		return;
	}

	WSSubsystem->IconSet = DefaultIconSet;
	WSSubsystem->IconSet.LoadSynchronous();
	if (!WSSubsystem->IconSet.IsValid())
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't load IconSet."));
		return;
	}

	WSSubsystem->Typography = OpenSansTypography;
	WSSubsystem->Typography.LoadSynchronous();
	if (!WSSubsystem->Typography.IsValid())
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't load Typography."));
		return;
	}

	WSSubsystem->BorderRadius = 35;
	WSSubsystem->IdealWidth = 94.f;
	WSSubsystem->IdealHeight = 35.f;

	WSSubsystem->SaveValuesToIni();
}

bool UWidgetStudioTestClass::TestGetTheme()
{
	if (!GEngine)
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't get GEngine."));
		return false;
	}
	if (!GConfig)
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't get GConfig."));
		return false;
	}
	WSSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	ResetTheme();

	WSSubsystem->Theme.Reset();
	//WSSubsystem->SetTheme(NordLightTheme);
	WSSubsystem->SetTheme(NordDarkTheme);

	TSoftObjectPtr<UWidgetStudioTheme> TestTheme;
	TestTheme = WSSubsystem->GetTheme();
	if (TestTheme != WSSubsystem->Theme)
	{
		ResetTheme();
		return false;
	}

	ResetTheme();
	return true;
}

bool UWidgetStudioTestClass::TestGetIconSet()
{
	if (!GEngine)
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't get GEngine."));
		return false;
	}
	if (!GConfig)
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't get GConfig."));
		return false;
	}
	WSSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	ResetTheme();

	WSSubsystem->IconSet.Reset();
	//WSSubsystem->SetIconSet(RobotoIconSet);
	WSSubsystem->SetIconSet(DefaultIconSet);

	TSoftObjectPtr<UWidgetStudioIconSet> TestIconSet;
	TestIconSet = WSSubsystem->GetIconSet();
	if (TestIconSet != WSSubsystem->IconSet)
	{
		ResetTheme();
		return false;
	}

	ResetTheme();
	return true;
}

bool UWidgetStudioTestClass::TestGetTypography()
{
	if (!GEngine)
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't get GEngine."));
		return false;
	}
	if (!GConfig)
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't get GConfig."));
		return false;
	}
	WSSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	ResetTheme();

	WSSubsystem->Typography.Reset();
	//WSSubsystem->SetTypography(OTHERTypography);
	WSSubsystem->SetTypography(OpenSansTypography);

	TSoftObjectPtr<UWidgetStudioTypography> TestTypography;
	TestTypography = WSSubsystem->GetTypography();
	if (TestTypography != WSSubsystem->Typography)
	{
		ResetTheme();
		return false;
	}

	ResetTheme();
	return true;
}

bool UWidgetStudioTestClass::TestGetBorderRadius()
{
	if (!GEngine)
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't get GEngine."));
		return false;
	}
	WSSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	ResetTheme();

	int32 SetRadius = 6;
	int32 ReadRadius = 0;

	WSSubsystem->SetBorderRadius(SetRadius);
	ReadRadius = WSSubsystem->GetBorderRadius();

	if (ReadRadius != WSSubsystem->BorderRadius)
	{
		ResetTheme();
		return false;
	}
	ResetTheme();
	return true;
}

bool UWidgetStudioTestClass::TestGetControlDimensions()
{
	if (!GEngine)
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't get GEngine."));
		return false;
	}
	WSSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	ResetTheme();

	FVector2D SetVector(33.f, 33.f);
	FVector2D ReadVector(0.f, 0.f);

	WSSubsystem->SetControlDimensions(SetVector);
	ReadVector = WSSubsystem->GetControlDimensions();
	FVector2D ControlDimensions(WSSubsystem->IdealWidth, WSSubsystem->IdealHeight);
	if (ReadVector != ControlDimensions)
	{
		ResetTheme();
		return false;
	}
	ResetTheme();
	return true;
}

bool UWidgetStudioTestClass::TestSetTheme()
{
	if (!GEngine)
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't get GEngine."));
		return false;
	}
	WSSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	ResetTheme();

	FString ReadTheme = TEXT("");
	FString NordDark = TEXT("/WidgetStudio/Style/Themes/Theme_Nord_Dark.Theme_Nord_Dark");

	FString Key(TEXT("Theme"));
	WSSubsystem->Theme.Reset();
	if (!GConfig)
	{
		ResetTheme();
		return false;
	}

	//GConfig->RemoveKey(*WSSubsystem->WfDefaultGameSection, *Key, WSSubsystem->DefaultGameIni);

	NordDarkTheme.LoadSynchronous();
	if (!NordDarkTheme.IsValid())
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't load Theme."));
		ResetTheme();
		return false;
	}
	WSSubsystem->SetTheme(NordDarkTheme);

	if (WSSubsystem->Theme != NordDarkTheme)
	{
		//UE_LOG(LogWidgetStudioTest, Warning, TEXT("ReadTheme = %s, NordDark = %s"), *ReadTheme, *NordDark);
		ResetTheme();
		return false;
	}

	//Check PrimaryAccent R
	if (WSSubsystem->Theme.Get()->PrimaryAccent.R != NordDarkTheme.Get()->PrimaryAccent.R)
	{
		//UE_LOG(LogWidgetStudioTest, Warning, TEXT("ThemeGetR = %f, NordDarkR = %f"), WSSubsystem->Theme.Get()->PrimaryAccent.R, NordDarkTheme.Get()->PrimaryAccent.R);
		ResetTheme();
		return false;
	}

	ResetTheme();
	return true;
}

bool UWidgetStudioTestClass::TestSetIconSet()
{
	if (!GEngine)
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't get GEngine."));
		return false;
	}
	WSSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	ResetTheme();

	FString ReadIconSet = TEXT("");
	FString DefaultIconSetString = TEXT("/WidgetStudio/Style/IconSets/IconSet_Default.IconSet_Default");

	FString Key(TEXT("IconSet"));
	WSSubsystem->IconSet.Reset();
	if (!GConfig)
	{
		ResetTheme();
		return false;
	}

	//GConfig->RemoveKey(*WSSubsystem->WfDefaultGameSection, *Key, WSSubsystem->DefaultGameIni);

	DefaultIconSet.LoadSynchronous();
	if (!DefaultIconSet.IsValid())
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't load Icons."));
		ResetTheme();
		return false;
	}

	WSSubsystem->SetIconSet(DefaultIconSet);

	if (WSSubsystem->IconSet != DefaultIconSet)
	{
		ResetTheme();
		return false;
	}

	//Check ArrowDown
	if (WSSubsystem->IconSet.Get()->ArrowDown != DefaultIconSet.Get()->ArrowDown)
	{
		ResetTheme();
		return false;
	}

	ResetTheme();
	return true;
}

bool UWidgetStudioTestClass::TestSetTypography()
{
	if (!GEngine)
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't get GEngine."));
		return false;
	}
	WSSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	ResetTheme();

	FString ReadTypography = TEXT("");
	FString OpenSans = TEXT("/WidgetStudio/Style/Typography/Typography_OpenSans.Typography_OpenSans");

	FString Key(TEXT("Typography"));
	WSSubsystem->Typography.Reset();
	if (!GConfig)
	{
		ResetTheme();
		return false;
	}

	//GConfig->RemoveKey(*WSSubsystem->WfDefaultGameSection, *Key, WSSubsystem->DefaultGameIni);

	OpenSansTypography.LoadSynchronous();
	if (!OpenSansTypography.IsValid())
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't load Typography."));
		ResetTheme();
		return false;
	}

	WSSubsystem->SetTypography(OpenSansTypography);

	if (WSSubsystem->Typography != OpenSansTypography)
	{
		ResetTheme();
		return false;
	}

	//Check LetterSpcaing
	if (WSSubsystem->Typography.Get()->Caption.LetterSpacing != OpenSansTypography.Get()->Caption.LetterSpacing)
	{
		ResetTheme();
		return false;
	}

	ResetTheme();
	return true;
}

bool UWidgetStudioTestClass::TestSetBorderRadius()
{
	if (!GEngine)
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't get GEngine."));
		return false;
	}
	WSSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	ResetTheme();

	int32 NewRadius = 18;
	WSSubsystem->SetBorderRadius(NewRadius);

	if (WSSubsystem->BorderRadius != NewRadius)
	{
		ResetTheme();
		return false;
	}
	ResetTheme();
	return true;
}

bool UWidgetStudioTestClass::TestSetControlDimensions()
{
	if (!GEngine)
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't get GEngine."));
		return false;
	}
	WSSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();

	float OldDimX = -1.f;
	float OldDimY = -1.f;
	FVector2D NewDimensions(44.f, 44.f);

	WSSubsystem->IdealWidth = OldDimX;
	WSSubsystem->IdealHeight = OldDimY;
	WSSubsystem->SetControlDimensions(NewDimensions);

	if (WSSubsystem->IdealWidth != NewDimensions.X || WSSubsystem->IdealHeight != NewDimensions.Y)
	{
		ResetTheme();
		return false;
	}
	ResetTheme();
	return true;
}

bool UWidgetStudioTestClass::TestResetTheme()
{
	if (!GEngine)
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't get GEngine."));
		return false;
	}
	WSSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();

	WSSubsystem->Theme.Reset();
	WSSubsystem->IconSet.Reset();
	WSSubsystem->Typography.Reset();
	WSSubsystem->BorderRadius = 333;
	WSSubsystem->IdealWidth = 33.3f;
	WSSubsystem->IdealHeight = 44.4f;

	WSSubsystem->SaveValuesToIni();

	ResetTheme();

	if (WSSubsystem->Theme != NordDarkTheme ||
		WSSubsystem->IconSet != DefaultIconSet ||
		WSSubsystem->Typography != OpenSansTypography ||
		WSSubsystem->BorderRadius != 35 ||
		WSSubsystem->IdealWidth != 94.f ||
		WSSubsystem->IdealHeight != 35.f)
	{
		ResetTheme();
		return false;
	}

	ResetTheme();
	return true;
}

bool UWidgetStudioTestClass::PerformAllTests()
{
	bool bDidTestPass;

	if (!GEngine)
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Couldn't get GEngine."));
		return false;
	}
	WSSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	ResetTheme();

	bDidTestPass = TestGetTheme();
	LogTestResult(TEXT("TestGetTheme()"), bDidTestPass, false);
	bDidTestPass = TestGetIconSet();
	LogTestResult(TEXT("TestGetIconSet()"), bDidTestPass, false);
	bDidTestPass = TestGetTypography();
	LogTestResult(TEXT("TestGetTypography()"), bDidTestPass, false);
	bDidTestPass = TestGetBorderRadius();
	LogTestResult(TEXT("TestGetBorderRadius()"), bDidTestPass, false);
	bDidTestPass = TestGetControlDimensions();
	LogTestResult(TEXT("TestGetControlDimensions()"), bDidTestPass, true);

	bDidTestPass = TestSetTheme();
	LogTestResult(TEXT("TestSetTheme()"), bDidTestPass, false);
	bDidTestPass = TestSetIconSet();
	LogTestResult(TEXT("TestSetIconSet()"), bDidTestPass, false);
	bDidTestPass = TestSetTypography();
	LogTestResult(TEXT("TestSetTypography()"), bDidTestPass, false);
	bDidTestPass = TestSetBorderRadius();
	LogTestResult(TEXT("TestSetBorderRadius()"), bDidTestPass, false);
	bDidTestPass = TestSetControlDimensions();
	LogTestResult(TEXT("TestSetControlDimensions()"), bDidTestPass, true);

	bDidTestPass = TestResetTheme();
	LogTestResult(TEXT("TestResetTheme()"), bDidTestPass, true);

	ResetTheme();
	return true;
}

void UWidgetStudioTestClass::LogTestResult(const FString TestName, const bool bDidTestPass, const bool bPlaceNewlineAfter)
{
	if (bDidTestPass)
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Test %s passed."), *TestName);
	}
	else
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("Test %s FAILED."), *TestName);
	}
	if (bPlaceNewlineAfter)
	{
		UE_LOG(LogWidgetStudioTest, Warning, TEXT("***"));
	}
}
