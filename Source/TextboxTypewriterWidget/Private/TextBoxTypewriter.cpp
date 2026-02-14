// (c) Noowai 2016-2026


#include "TextBoxTypewriter.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void UTextBoxTypewriter::StartTypewriting()
{
	if(TH_TextTyping.IsValid() || SourceString.IsEmpty())
		return; // don't start typewriting if it's already going on
		
	if(SourceCharacters.Num()<=0)
		ensure(false);	
	
	const float ActualSpeed = bIsSpedUp ? SpeedUpCoeff:1.f * Speed;
	float LetterDelay = 1/ActualSpeed;
	if(IsValid(GetWorld()))
		GetWorld()->GetTimerManager().SetTimer(TH_TextTyping, this, &UTextBoxTypewriter::AddLetter, LetterDelay, true, DelayAtStart);
}

void UTextBoxTypewriter::SwitchSpeedUp(bool bShouldSpeedUp)
{
	bIsSpedUp = bShouldSpeedUp;
}

void UTextBoxTypewriter::SkipTypewriting()
{
	UpdateText(FText::FromString(SourceString));
	FinishTyping();
}

void UTextBoxTypewriter::SetText(FText InText)
{
	if(IsDesignTime())
		Super::SetText(InText);
	else
	{	
		SourceString = InText.ToString();
		SourceCharacters = InText.ToString().GetCharArray();
		if(bAutoStart)
			StartTypewriting();
		Super::SetText(FText::FromString(""));
	}	
}

void UTextBoxTypewriter::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	SourceString = Text.ToString();
	SourceCharacters = Text.ToString().GetCharArray();
	if(!IsDesignTime())
	{
		UpdateText(FText::FromString(""));
		if(bAutoStart)
			StartTypewriting();
	}
}

void UTextBoxTypewriter::AddLetter()
{
	const TCHAR& Letter = SourceCharacters.GetData()[LetterPointer];
	CurrentString.AppendChar(Letter);
	LetterPointer++;

	if(IsValid(TypeSound) && Letter != *" ")
		UGameplayStatics::PlaySound2D(this, TypeSound);
	
	UpdateText(FText::FromString(CurrentString));

	if(!SourceCharacters.IsValidIndex(LetterPointer))
	{
		FinishTyping();
		return;
	}

	if(bShouldPause && !bIsSpedUp && PauseOnCharacters.GetCharArray().Contains(Letter))
	{
		bWasPaused = true;
		GetWorld()->GetTimerManager().SetTimer(TH_TextTyping, this, &UTextBoxTypewriter::AddLetter, PauseTime, false);
		return;
	}

	if(bWasPaused)
	{
		bWasPaused = false;
		const float ActualSpeed = bIsSpedUp ? SpeedUpCoeff:1.f * Speed;
		float LetterDelay = 1/ActualSpeed;
		GetWorld()->GetTimerManager().SetTimer(TH_TextTyping, this, &UTextBoxTypewriter::AddLetter, LetterDelay, true);
	}
}

void UTextBoxTypewriter::FinishTyping()
{
	GetWorld()->GetTimerManager().ClearTimer(TH_TextTyping);
	TH_TextTyping.Invalidate();
	CurrentString = "";
	SourceCharacters.Empty();
	LetterPointer = 0;
	OnTypingComplete.Broadcast();
}

void UTextBoxTypewriter::UpdateText(FText InText)
{
	Text = InText;
	TextDelegate.Unbind();
	if ( MyTextBlock.IsValid() )
	{
		TAttribute<FText> TextBinding = GetDisplayText();
		MyTextBlock->SetText(TextBinding);
	}
}
