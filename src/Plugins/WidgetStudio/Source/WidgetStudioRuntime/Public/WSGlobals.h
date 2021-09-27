#pragma once

#include "Containers/UnrealString.h"
#include "Misc/Paths.h"

FString DefaultGameIni = FPaths::ProjectConfigDir() + TEXT("/DefaultGame.ini");
FString WSDefaultGameSection = TEXT("/Script/WidgetStudioEditor.WidgetStudioSettings");

FString StringTheme = TEXT("Theme");
FString StringIconSet = TEXT("IconSet");
FString StringTypography = TEXT("Typography");
FString StringBorderRadius = TEXT("BorderRadius");
FString StringIdealWidth = TEXT("IdealWidth");
FString StringIdealHeight = TEXT("IdealHeight");
FString StringControlDimensions = TEXT("ControlDimensions");

FString DefaultTheme = TEXT("/WidgetStudio/Style/Themes/Theme_Mint.Theme_Mint");
FString DefaultIconSet = TEXT("/WidgetStudio/Style/IconSets/IconSet_Default.IconSet_Default");
FString DefaultTypography = TEXT("/WidgetStudio/Style/Typography/Typography_OpenSans.Typography_OpenSans");
int32 DefaultBorderRadius = 15;
float DefaultIdealWidth = 75.f;
float DefaultIdealHeight = 35.f;