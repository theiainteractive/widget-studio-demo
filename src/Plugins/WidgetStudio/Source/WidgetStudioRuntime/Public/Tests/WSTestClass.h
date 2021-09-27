#pragma once

#include "CoreMinimal.h"
#include "WSSubsystem.h"
#include "WSTestClass.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWidgetStudioTest, All, All);

/**
 * Test class to perform unit tests
 */
UCLASS(Blueprintable, BlueprintType)
class WIDGETSTUDIORUNTIME_API UWidgetStudioTestClass : public UObject
{
	GENERATED_BODY()

public:
	UWidgetStudioTestClass();
	~UWidgetStudioTestClass();

	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Test")
	bool PerformAllTests();

	static void LogTestResult(FString TestName, bool bDidTestPass, bool bPlaceNewlineAfter);
	void ResetTheme();

	bool TestGetTheme();
	bool TestGetIconSet();
	bool TestGetTypography();
	bool TestGetBorderRadius();
	bool TestGetControlDimensions();

	bool TestSetTheme();
	bool TestSetIconSet();
	bool TestSetTypography();
	bool TestSetBorderRadius();
	bool TestSetControlDimensions();

	bool TestResetTheme();

	UWidgetStudioSubsystem *WSSubsystem = nullptr;

	TSoftObjectPtr<UWidgetStudioTheme> NordDarkTheme;
	TSoftObjectPtr<UWidgetStudioTheme> NordLightTheme;
	TSoftObjectPtr<UWidgetStudioIconSet> DefaultIconSet;
	TSoftObjectPtr<UWidgetStudioTypography> OpenSansTypography;
};
