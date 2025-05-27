#include "UI/CustomizableTextBox.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UCustomizableTextBox::InitializeWidget(const FText& Text, const float CustomTypingSpeed)
{
	ContentText = Text;
	CurrentCharIndex = 0;
	TypingSpeed = CustomTypingSpeed;
	StartTypewritingAnimation();
}

void UCustomizableTextBox::TypeNextCharacter()
{
	if (CurrentCharIndex > ContentText.ToString().Len())
	{
		GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
		ChangePlayImageState(false);
		return;
	}

	if (ContentTextBlock != nullptr)
	{
		const FString PartialText = ContentText.ToString().Left(CurrentCharIndex);
		const FString FormattedPartialText = InsertLineBreaks(PartialText, 60);
		ContentTextBlock->SetText(FText::FromString(FormattedPartialText));
		CurrentCharIndex++;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UCustomizableTextBox::TypeNextCharacter - ContentTextBlock is null. Unable to process the text to be displayed into the text block."));
	}
}

void UCustomizableTextBox::ChangePlayImageState(const bool IsEnable)
{
	const TObjectPtr<UImage> CurrentPlayImage = IsEnable ? PlayImage : PauseImage;
	if (AnimationPlayingSwitcher != nullptr)
	{
		AnimationPlayingSwitcher->SetActiveWidget(CurrentPlayImage);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UCustomizableTextBox::ChangePlayImageState - AnimationPlayingSwitcher is null. Unable to change the animation playing image to a new state."));
	}
}

void UCustomizableTextBox::StartTypewritingAnimation()
{
	if (ContentTextBlock != nullptr)
	{
		ContentTextBlock->SetVisibility(ESlateVisibility::Visible);
		
		CurrentCharIndex = 1;
		GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
		ChangePlayImageState(true);

		GetWorld()->GetTimerManager().SetTimer(TypingTimerHandle, this,	&UCustomizableTextBox::TypeNextCharacter, TypingSpeed, true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UCustomizableTextBox::StartTypewritingAnimation - ContentTextBlock is null. Unable to start the typewriting animation to show the text."));
	}
}

FString UCustomizableTextBox::InsertLineBreaks(const FString& InputString, int32 InputLength)
{
	FString Result;
	int32 Start = 0;
	int32 Length = InputString.Len();

	while (Start < Length)
	{
		int32 Remaining = Length - Start;

		if (Remaining <= InputLength)
		{
			Result += InputString.Mid(Start, Remaining);
			break;
		}

		FString Segment = InputString.Mid(Start, InputLength);
		int32 BreakIndex;
		if (Segment.FindLastChar(TEXT(' '), BreakIndex))
		{
			Result += InputString.Mid(Start, BreakIndex) + TEXT("\n");
			Start += BreakIndex + 1;
		}
		else
		{
			Result += Segment + TEXT("\n");
			Start += InputLength;
		}
	}

	return Result;
}
