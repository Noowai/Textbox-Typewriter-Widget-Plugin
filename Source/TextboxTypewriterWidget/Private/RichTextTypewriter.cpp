// (c) Noowai 2016-2026


#include "RichTextTypewriter.h"

#include "SRichTextBlock.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


void URichTextTypewriter::StartTypewriting()
{
	if(TH_TextTyping.IsValid() || SourceString.IsEmpty())
		return; // don't start typewriting if it's already going on
		
	if(SourceCharacters.Num()<=0)
		ensure(false);

	if(bShowFullText)
	{
		SetText(FText::FromString(SourceString));
		return;
	}
	
	const float ActualSpeed = bIsSpedUp ? SpeedUpCoeff:1.f * Speed;
	float LetterDelay = 1/ActualSpeed;
	if(IsValid(GetWorld()))
		GetWorld()->GetTimerManager().SetTimer(TH_TextTyping, this, &URichTextTypewriter::AddLetter, LetterDelay, true, DelayAtStart);
}

void URichTextTypewriter::SwitchSpeedUp(bool bShouldSpeedUp)
{
	bIsSpedUp = bShouldSpeedUp;
}

void URichTextTypewriter::SkipTypewriting()
{
	UpdateText(FText::FromString(SourceString));
	FinishTyping();
}

void URichTextTypewriter::SetShowFullText(bool bShow)
{
	bShowFullText = bShow;
	if(bShow)
		SkipTypewriting();
}

void URichTextTypewriter::SetText(const FText& InText)
{
	if(IsDesignTime())
		Super::SetText(InText);
	else
	{	
		SourceString = InText.ToString();
		SourceCharacters = InText.ToString().GetCharArray();
		CurrentString.Reserve(SourceString.GetAllocatedSize());
		if(bAutoStart)
			StartTypewriting();
		Super::SetText(FText::FromString(""));
	}	
}

void URichTextTypewriter::SynchronizeProperties()
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

void URichTextTypewriter::AddLetter()
{
	bool bIsDecorator = false;
	TCHAR& Letter = SourceCharacters.GetData()[LetterPointer];
	
	if(Letter == '<')
	{
		if (bHasStyle)
		{
			LetterPointer+=2;
			bHasStyle = false;
		}
		else
		{
			CurrentString.AppendChar(Letter);
			do
			{
				LetterPointer++;
				Letter = SourceCharacters.GetData()[LetterPointer]; 
				CurrentString.AppendChar(Letter);
				if(Letter == '/')
					bIsDecorator = true;
			}
			while(Letter != '>' && LetterPointer < SourceString.Len());
		
			bHasStyle = !bIsDecorator; // only mark styled text if it wasn't decorator tag
			if(bHasStyle)
				CurrentString.Append("</>");
		}		
		LetterPointer++;
	}

	Letter = SourceCharacters.GetData()[LetterPointer];	
	if(Letter != '<') // in case we have consequent decorator don't instanlty add this letter
	{
		if(bHasStyle)	
			CurrentString.RemoveAt(CurrentString.Len()-3,3);
	
		CurrentString.AppendChar(Letter);
		LetterPointer++;	
	
		if(bHasStyle)	
			CurrentString.Append("</>");
	}

	if(IsValid(TypeSound) && (Letter != *" " || bIsDecorator))
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
		GetWorld()->GetTimerManager().SetTimer(TH_TextTyping, this, &URichTextTypewriter::AddLetter, PauseTime, false);
		return;
	}

	if(bWasPaused)
	{
		bWasPaused = false;
		const float ActualSpeed = bIsSpedUp ? SpeedUpCoeff:1.f * Speed;
		float LetterDelay = 1/ActualSpeed;
		GetWorld()->GetTimerManager().SetTimer(TH_TextTyping, this, &URichTextTypewriter::AddLetter, LetterDelay, true);
	}
}

void URichTextTypewriter::FinishTyping()
{
	GetWorld()->GetTimerManager().ClearTimer(TH_TextTyping);
	TH_TextTyping.Invalidate();
	CurrentString = "";
	SourceCharacters.Empty();
	LetterPointer = 0;
	OnTypingComplete.Broadcast();
}

void URichTextTypewriter::UpdateText(const FText& InText)
{
	Text = InText;
	if ( MyRichTextBlock.IsValid() )
	{
		MyRichTextBlock->SetText(InText);
	}
}
