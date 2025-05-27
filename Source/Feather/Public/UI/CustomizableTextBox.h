#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomizableTextBox.generated.h"

// Forward declarations
class UImage;
class UTextBlock;
class UWidgetSwitcher;

UCLASS()
class FEATHER_API UCustomizableTextBox : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeWidget(const FText& Text, const float CustomTypingSpeed = 0.03f);
	
private:
	FString InsertLineBreaks(const FString& InputString, int32 InputLength = 60);
	void TypeNextCharacter();
	void ChangePlayImageState(const bool IsEnable);
	void StartTypewritingAnimation();
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ContentTextBlock;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> AnimationPlayingSwitcher;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PlayImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PauseImage;
	
	UPROPERTY(EditDefaultsOnly)
	FText ContentText;
	FTimerHandle TypingTimerHandle;
	int32 CurrentCharIndex;
	float TypingSpeed;
};
