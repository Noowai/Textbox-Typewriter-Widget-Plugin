// (c) Noowai 2016-2026

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlock.h"
#include "RichTextTypewriter.generated.h"

/**
 * 
 */
UCLASS()
class URichTextTypewriter : public URichTextBlock
{
	GENERATED_BODY()
	
public:

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTypingComplete);

#pragma region Config
	// Speed in letters per second
	UPROPERTY(EditAnywhere, Category = "Typewriter")
	float Speed = 10.f;

	// Coefficient to speed up text appearing, for example when button is pressed to skip slowly appearing text
	UPROPERTY(EditAnywhere, Category = "Typewriter")
	float SpeedUpCoeff = 2.f;

	// Delay before starting typewrite
	UPROPERTY(EditAnywhere, Category = "Typewriter")
	float DelayAtStart = 0.3f;	

	// Sound for each symbol
	UPROPERTY(EditAnywhere, Category = "Typewriter")
	USoundBase* TypeSound = nullptr;	

	// should ever pause on any symbols
	UPROPERTY(EditAnywhere, Category = "Typewriter")
	bool bShouldPause = false;

	// time to pause on the symbol
	UPROPERTY(EditAnywhere, Category = "Typewriter", meta = (EditCondition=bShouldPause))
	float PauseTime = 0.3f;

	// list of symbols to pause on
	UPROPERTY(EditAnywhere, Category = "Typewriter", meta = (EditCondition=bShouldPause))
	FString PauseOnCharacters = ".,!?;:";	

	// If autostart is off, use StartTypewriting()
	UPROPERTY(EditAnywhere, Category = "Typewriter", AdvancedDisplay)
	bool bAutoStart = true;	

	// Don't typewrite, just show text
	UPROPERTY(EditAnywhere, Category = "Typewriter", AdvancedDisplay)
	bool bShowFullText = false;

#pragma endregion

	UPROPERTY(BlueprintAssignable)
	FOnTypingComplete OnTypingComplete;

	UFUNCTION(BlueprintCallable, Category = "Typewriter")
	void StartTypewriting();
	
	UFUNCTION(BlueprintCallable, Category = "Typewriter")
	void SwitchSpeedUp(bool bShouldSpeedUp);
	
	UFUNCTION(BlueprintCallable, Category = "Typewriter")
	void SkipTypewriting();
	
	UFUNCTION(BlueprintCallable, Category = "Typewriter")
	void SetShowFullText(bool bShow);

#pragma region Overrides
	virtual void SetText(const FText& InText) override;
	virtual void SynchronizeProperties() override;
	
#pragma endregion 
	
private:	
	UFUNCTION()
	void AddLetter();

	void FinishTyping();
	void UpdateText(const FText& InText);

	bool DetectStyleTag(const FChar& Letter);

	bool bIsSpedUp = false;
	bool bWasPaused = false;
	FTimerHandle TH_TextTyping;
	TArray<TCHAR> SourceCharacters;
	FString SourceString;
	FString CurrentString = "";
	int32 LetterPointer = 0;
	bool bHasStyle = false;
};
