#include "Misc/AutomationTest.h"
#include "Engine/Engine.h"
#include "Tests/WSTestClass.h"

#if WITH_DEV_AUTOMATION_TESTS

//AT BEGINNING, SAVE CONTENTS OF INI FILES
//AT END, RESTORE INI FILES TO ORIGINAL STATE

BEGIN_DEFINE_SPEC(FunctionsSpec, "WidgetStudio.Functions",
	EAutomationTestFlags::ProductFilter |
	EAutomationTestFlags::EditorContext /*PIE?*/ |
	EAutomationTestFlags::ClientContext |
	EAutomationTestFlags::ServerContext)

	UWidgetStudioTestClass* TestClass = nullptr;

END_DEFINE_SPEC(FunctionsSpec)

void FunctionsSpec::Define()
{
	BeforeEach([this]()
	{
		if (TestClass == nullptr)
		{
			TestClass = NewObject<UWidgetStudioTestClass>();
		}
	});
	AfterEach([this]() {});

	Describe("BlueprintApiGetters()", [this]()
	{
		Describe("GetTheme()", [this]()
		{
			It("should test that a theme is properly retrieved", [this]()
			{
				const bool bDidTestPass = TestClass->TestGetTheme();
				TestTrue("GetTheme()", bDidTestPass);
			});
		});
		Describe("GetIconSet()", [this]()
		{
			It("should test that an IconSet is properly retrieved", [this]()
			{
				const bool bDidTestPass = TestClass->TestGetIconSet();
				TestTrue("GetIconSet()", bDidTestPass);
			});
		});
		Describe("GetTypography()", [this]()
		{
			It("should test that a Typography is properly retrieved", [this]()
			{
				const bool bDidTestPass = TestClass->TestGetTypography();
				TestTrue("GetTypography()", bDidTestPass);
			});
		});
		Describe("GetBorderRadius()", [this]()
		{
			It("should test that a BorderRadius is properly retrieved", [this]()
			{
				const bool bDidTestPass = TestClass->TestGetBorderRadius();
				TestTrue("GetBorderRadius()", bDidTestPass);
			});
		});
		Describe("GetControlDimensions()", [this]()
		{
			It("should test that ControlDimensions are properly retrieved", [this]()
			{
				const bool bDidTestPass = TestClass->TestGetControlDimensions();
				TestTrue("GetControlDimensions()", bDidTestPass);
			});
		});
	});

	Describe("BlueprintApiSetters()", [this]()
	{
		Describe("SetTheme()", [this]()
		{
			It("should test that a theme is properly set", [this]()
			{
				const bool bDidTestPass = TestClass->TestSetTheme();
				TestTrue("SetTheme()", bDidTestPass);
			});
		});
		Describe("SetIconSet()", [this]()
		{
			It("should test that an IconSet is properly set", [this]()
			{
				const bool bDidTestPass = TestClass->TestSetIconSet();
				TestTrue("SetIconSet()", bDidTestPass);
			});
		});
		Describe("SetTypography()", [this]()
		{
			It("should test that a Typography is properly set", [this]()
			{
				const bool bDidTestPass = TestClass->TestSetTypography();
				TestTrue("SetTypography()", bDidTestPass);
			});
		});
		Describe("SetBorderRadius()", [this]()
		{
			It("should test that a BorderRadius is properly set", [this]()
			{
				const bool bDidTestPass = TestClass->TestSetBorderRadius();
				TestTrue("SetBorderRadius()", bDidTestPass);
			});
		});
		Describe("SetControlDimensions()", [this]()
		{
			It("should test that ControlDimensions are properly set", [this]()
			{
				const bool bDidTestPass = TestClass->TestSetControlDimensions();
				TestTrue("SetControlDimensions()", bDidTestPass);
			});
		});
	});

	Describe("Misc()", [this]()
	{
		Describe("ResetTheme()", [this]()
		{
			It("should reset Theme, IconSet, Typography, BorderRadius, and ControlDimensions to default values", [this]()
			{
				const bool bDidTestPass = TestClass->TestResetTheme();
				TestTrue("ResetTheme()", bDidTestPass);
			});
		});
	});
}

#endif //WITH_DEV_AUTOMATION_TESTS
