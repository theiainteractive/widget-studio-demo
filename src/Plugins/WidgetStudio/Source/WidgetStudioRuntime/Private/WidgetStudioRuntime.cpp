// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "WidgetStudioRuntime.h"

#define LOCTEXT_NAMESPACE "FWidgetStudioRuntime"
DEFINE_LOG_CATEGORY(LogWidgetStudio);

void FWidgetStudioRuntime::StartupModule()
{
}

void FWidgetStudioRuntime::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWidgetStudioRuntime, WidgetStudioRuntime)
