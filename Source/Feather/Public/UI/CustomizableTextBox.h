#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomizableTextBox.generated.h"

// Forward declarations
class UImage;
class UTextBlock;
class UWidgetSwitcher;

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	BUNNY,
	KIWI,
	SQUIRREL
};

USTRUCT(BlueprintType)
struct FDialogConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	ECharacterType CharacterType = ECharacterType::KIWI;
	
	UPROPERTY(EditDefaultsOnly)
	FText Text = FText::FromString("");
	
	UPROPERTY(EditDefaultsOnly)
	float CustomTypingSpeed = 0.03;
};

UCLASS()
class FEATHER_API UCustomizableTextBox : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ClickAction();
	UFUNCTION(BlueprintCallable)
	void StartDialog(const TArray<FDialogConfig>& DialogConfig);
	
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
	
	UPROPERTY(EditDefaultsOnly)
	FText ContentText;
	FTimerHandle TypingTimerHandle;
	int32 CurrentCharIndex;
	float TypingSpeed;
	int32 CurrentDialogIndex;
	bool bIsWaitingForNextDialog;
	
	UPROPERTY()
	TArray<FDialogConfig> Dialogs;
};
