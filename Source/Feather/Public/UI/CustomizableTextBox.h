#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ECharacterType.h"
#include "FDialogConfiguration.h"
#include "CustomizableTextBox.generated.h"

// Forward declarations
class UImage;
class UTextBlock;
class UWidgetSwitcher;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishedDialogDelegate);

UCLASS()
class FEATHER_API UCustomizableTextBox : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ClickAction();
	UFUNCTION(BlueprintCallable)
	void StartDialog(const TArray<FDialogConfiguration>& DialogConfig);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnFinishedDialogDelegate OnFinishedDialogDelegate;
	
private:
	FString InsertLineBreaks(const FString& InputString, int32 InputLength = 60);
	void TypeNextCharacter();
	void ChangePlayImageState(const bool IsEnable);
	void CompleteTextInstantly();
	void ShowCurrentDialog();
	void StartTypewritingAnimation();
	void UpdateCharacterImage(ECharacterType CharacterType);
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ContentTextBlock;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> AnimationPlayingSwitcher;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> CharacterImagesSwitcher;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PlayImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PauseImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> BunnyImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> KiwiImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SquirrelImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CrowImage;
	
	UPROPERTY(EditDefaultsOnly)
	FText ContentText;
	FTimerHandle TypingTimerHandle;
	int32 CurrentCharIndex;
	float TypingSpeed;
	int32 CurrentDialogIndex;
	bool bIsWaitingForNextDialog;
	
	UPROPERTY()
	TArray<FDialogConfiguration> Dialogs;
};
