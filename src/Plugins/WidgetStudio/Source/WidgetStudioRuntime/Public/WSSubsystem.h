#pragma once

#include "CoreMinimal.h"

/* used for log category - do not remove */
#include "WidgetStudioRuntime.h"

#include "Subsystems/EngineSubsystem.h"
#include "Theme/WSIconSet.h"
#include "Theme/WSTheme.h"
#include "Theme/WSTypography.h"
#include "WSSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStyleDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartInitDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndInitDelegate);

//To access:  UWidgetStudioSubsystem* WidgetStudio = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();

/**
 * A singleton for managing widget styles and managing widget state instantiates when the plugin loads.
 */
UCLASS(Blueprintable, BlueprintType, Category = "Widget Studio")
class WIDGETSTUDIORUNTIME_API UWidgetStudioSubsystem final : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	/*
	 * Settings
	 */

	UPROPERTY(EditAnywhere, Category = "Theme", Meta = (HideViewOptions, DisplayThumbnail = "false", ToolTip = "Choose the current Widget Studio theme"))
	TSoftObjectPtr<UWidgetStudioTheme> Theme;

	UPROPERTY(EditAnywhere, Category = "Typography", Meta = (HideViewOptions, ToolTip = "Choose the font/type look and feel"))
	TSoftObjectPtr<UWidgetStudioTypography> Typography;

	UPROPERTY(EditAnywhere, Category = "Icon Set", Meta = (HideViewOptions, ToolTip = "Choose the icon style"))
	TSoftObjectPtr<UWidgetStudioIconSet> IconSet;

	UPROPERTY(EditAnywhere, Category = "Style", Meta = (ToolTip = "The corner roundness found on controls, such as buttons, comboboxs, etc."))
	int32 BorderRadius = 15;

	UPROPERTY(EditAnywhere, Category = "Style", Meta = (ToolTip = "The default x dimension of controls if not overridden via layouts."))
	float IdealWidth = 325.f;

	UPROPERTY(EditAnywhere, Category = "Style", Meta = (ToolTip = "The default y dimension of controls if not overridden via layouts."))
	float IdealHeight = 35.f;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, BlueprintCallable, Category = "Widget Studio|Events")
	FStyleDelegate OnStyleChanged;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, BlueprintCallable, Category = "Widget Studio|Inititalization")
	FStartInitDelegate OnPluginStartedInit;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, BlueprintCallable, Category = "Widget Studio|Initialization")
	FEndInitDelegate OnPluginFinishedInit;

	// Begin BP Interface
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Style")
	UWidgetStudioTheme *GetTheme();

	UFUNCTION(BlueprintPure, Category = "Widget Studio|Style")
	UWidgetStudioIconSet *GetIconSet();

	UFUNCTION(BlueprintPure, Category = "Widget Studio|Style")
	UWidgetStudioTypography *GetTypography();

	UFUNCTION(BlueprintPure, Category = "Widget Studio|Style")
	static int32 GetBorderRadius();

	UFUNCTION(BlueprintPure, Category = "Widget Studio|Style")
	static FVector2D GetControlDimensions();

	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Style")
	bool SetTheme(TSoftObjectPtr<UWidgetStudioTheme> InTheme);

	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Style")
	bool SetIconSet(TSoftObjectPtr<UWidgetStudioIconSet> InIconStyle);

	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Style")
	bool SetTypography(TSoftObjectPtr<UWidgetStudioTypography> InTypography);

	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Style")
	bool SetBorderRadius(int32 InRadius);

	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Style")
	bool SetControlDimensions(FVector2D InDimensions);

	UFUNCTION(BlueprintPure, Category = "Widget Studio|Initialization")
	bool IsPluginInitialized() const
	{
		return bIsPluginInitialized;
	}
	//End BP Interface

	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase &Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	static UWidgetStudioSubsystem *GetInstance();

private:
	friend class UWidgetStudioTestClass;

	UWidgetStudioSubsystem();
	virtual ~UWidgetStudioSubsystem() override;

	void LoadValuesFromIni();
	void SaveValuesToIni() const;

	void HandleAssetRenamed(const FAssetData& AssetData, const FString& NewName);
	void HandleAssetAdded(const FAssetData& AssetData);

	bool bIsPluginInitialized = false;
	bool bIsDataAssetRenameInProcess = false;

	FString ChangedDataAssetName = TEXT("");
	FString ChangedDataAssetType = TEXT("");

	static UWidgetStudioSubsystem *Instance;
};
