// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/WSContainer.h"

void UWidgetStudioContainer::ConstructOption(FButtonOptions Option)
{
}

void UWidgetStudioContainer::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

int32 UWidgetStudioContainer::GetCurrentIndex() const
{
	return CurrentIndex;
}

int32 UWidgetStudioContainer::GetOptionCount() const
{
	return Options.Num();
}

TArray<FButtonOptions> UWidgetStudioContainer::GetOptions() const
{
	return Options;
}

FButtonOptions UWidgetStudioContainer::GetOptionAtIndex(const int32 Index)
{
	if (Options.IsValidIndex(Index))
	{
		return Options[Index];
	}

	return FButtonOptions();
}

FButtonOptions UWidgetStudioContainer::GetCurrentOption() const
{
	if (Options.IsValidIndex(CurrentIndex))
	{
		return Options[CurrentIndex];
	}
	return FButtonOptions();
}

int32 UWidgetStudioContainer::GetOptionIndexViaText(const FText InText)
{
	for (int32 i = 0; i < Options.Num(); i++)
	{
		if (Options[i].Text.ToString().ToLower() == InText.ToString().ToLower())
		{
			return i;
		}
	}

	return -1;
}

void UWidgetStudioContainer::IncrementCurrentIndex()
{
	SetCurrentIndex(FMath::Clamp(CurrentIndex + 1, 0, Options.Num()));
}

void UWidgetStudioContainer::DecrementCurrentIndex()
{
	SetCurrentIndex(FMath::Clamp(CurrentIndex - 1, 0, Options.Num()));
}

void UWidgetStudioContainer::ClearCurrentIndex()
{
	SetCurrentIndex(-1);
}

void UWidgetStudioContainer::SetOptions(const TArray<FButtonOptions> NewOptions)
{
	ClearOptions();
	Options = NewOptions;

	for (int32 i = 0; i < Options.Num(); i++)
	{
		ConstructOption(Options[i]);
	}
}

void UWidgetStudioContainer::SetCurrentIndex(const int32 Index)
{
	if (Index > -1 && Index < Options.Num())
	{
		CurrentIndex = Index;
		OnCurrentIndexChanged.Broadcast(CurrentIndex, Options[CurrentIndex]);
	}
	else
	{
		CurrentIndex = -1;
		OnCurrentIndexChanged.Broadcast(CurrentIndex, FButtonOptions());
	}
}

void UWidgetStudioContainer::AddOption(const FButtonOptions NewOption)
{
	Options.Add(NewOption);
	ConstructOption(NewOption);
}

void UWidgetStudioContainer::ClearOptions()
{
	Options.Empty();
	CurrentIndex = -1;
}

bool UWidgetStudioContainer::SetCurrentIndexViaOptionText(const FText InText)
{
	for (int32 i = 0; i < Options.Num(); i++)
	{
		if (Options[i].Text.ToString().ToLower() == InText.ToString().ToLower())
		{
			SetCurrentIndex(i);
			return true;
		}
	}
	return false;
}
