// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WSBase.h"

void UWidgetStudioBase::InitializeStyling()
{
}

void UWidgetStudioBase::UpdateStyling()
{
}

FVector2D UWidgetStudioBase::GetDimensions() const
{
	const FVector2D ControlDimensions = UWidgetStudioFunctionLibrary::GetControlDimensions();

	const float Width = UWidgetStudioFunctionLibrary::GetSizeByModifier(SizeModifier, OverrideDimensions.X > 0 ? OverrideDimensions.X : ControlDimensions.X);
	const float Height = UWidgetStudioFunctionLibrary::GetSizeByModifier(SizeModifier, OverrideDimensions.Y > 0 ? OverrideDimensions.Y : ControlDimensions.Y);
	
	return FVector2D(
		Width > MinimumDimensions.X ? Width : MinimumDimensions.X,
		Height > MinimumDimensions.Y ? Height : MinimumDimensions.Y
		);
}

void UWidgetStudioBase::ForceStyleUpdate()
{
	UpdateStyling();
}
