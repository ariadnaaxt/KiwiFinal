#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ComicSimulationWidget.generated.h"

// Forward declarations
class UTexture2D;
class UAnimationAsset;
class UImage;

UCLASS()
class FEATHER_API UComicSimulationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void StartComicSimulation(const TArray<UTexture2D*>& InTextures);

	UFUNCTION(BlueprintCallable)
	void ClickAction();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayFadeOutAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayFadeInAnimation();
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image;

	UPROPERTY()
	TArray<TObjectPtr<UTexture2D>> ComicTextures;

	int32 CurrentIndex = 0;
	bool bIsTransitioning = false;

	UFUNCTION()
	void OnFadeOutFinished();

	UFUNCTION()
	void OnFadeInFinished();

	void SetImageTexture(UTexture2D* Texture);
};
