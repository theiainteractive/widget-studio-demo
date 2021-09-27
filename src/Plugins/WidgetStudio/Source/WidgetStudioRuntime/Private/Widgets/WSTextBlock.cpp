#include "Widgets/WSTextBlock.h"

void UWidgetStudioTextBlock::SetWrapTextAt(const float InWrapTextAt)
{
	WrapTextAt = InWrapTextAt;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetWrapTextAt(InWrapTextAt);
	}
}

void UWidgetStudioTextBlock::SetWrappingPolicy(const ETextWrappingPolicy InWrappingPolicy)
{
	WrappingPolicy = InWrappingPolicy;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetWrappingPolicy(InWrappingPolicy);
	}
}