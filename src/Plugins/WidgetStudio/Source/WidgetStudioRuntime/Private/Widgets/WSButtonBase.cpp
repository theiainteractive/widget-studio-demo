// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WSButtonBase.h"
#include "InputCoreTypes.h"
#include "TimerManager.h"

FReply UWidgetStudioButtonBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!bIsInteractable)
	{
		return FReply::Unhandled();
	}
	
	if (InMouseEvent.IsMouseButtonDown(FKey("LeftMouseButton")))
	{
		bIsPressed = true;
		
		if (IsCheckable())
		{
			SetChecked(!IsChecked());
			OnToggled.Broadcast(this, IsChecked());
		}
		else
		{
			if (GetWorld()->GetTimerManager().IsTimerActive(DoublePressTimer))
			{
				OnDoublePressed.Broadcast(this);
				GetWorld()->GetTimerManager().ClearTimer(DoublePressTimer);
			}
			else
			{
				GetWorld()->GetTimerManager().SetTimer(DoublePressTimer, 0.65f, false);
			}
		}
		
		OnPressed.Broadcast(this);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply UWidgetStudioButtonBase::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsPressed = false;
	OnReleased.Broadcast(this);
	return FReply::Handled();
}

void UWidgetStudioButtonBase::NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{
	bIsPressed = false;
}

void UWidgetStudioButtonBase::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	bIsPressed = false;
}

void UWidgetStudioButtonBase::SetCheckable(const bool bNewCheckableState)
{
	if(bForceCheckable)
	{
		bIsCheckable = true;
	}
	else
	{
		bIsCheckable = bNewCheckableState;
	}
}

void UWidgetStudioButtonBase::SetChecked(const bool bNewCheckedState)
{
	if (bIsCheckable || bForceCheckable)
	{
		bIsChecked = bNewCheckedState;
	}
}

bool UWidgetStudioButtonBase::IsCheckable() const
{
	return bIsCheckable;
}

bool UWidgetStudioButtonBase::IsChecked() const
{
	return bIsCheckable && bIsChecked;
}

bool UWidgetStudioButtonBase::IsPressed() const
{
	return bIsPressed;
}
