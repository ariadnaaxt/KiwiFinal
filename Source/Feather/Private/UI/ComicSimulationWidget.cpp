#include "UI/ComicSimulationWidget.h"
#include "Components/Image.h"
#include "TimerManager.h"

void UComicSimulationWidget::StartComicSimulation(const TArray<UTexture2D*>& InTextures)
{
	if (InTextures.Num() == 0 || !Image) return;

	ComicTextures = InTextures;
	CurrentIndex = 0;
	bIsTransitioning = false;

	SetImageTexture(ComicTextures[CurrentIndex]);
	PlayFadeInAnimation();
}

void UComicSimulationWidget::ClickAction()
{
	if (bIsTransitioning || !ComicTextures.IsValidIndex(CurrentIndex + 1))
		return;

	bIsTransitioning = true;
	PlayFadeOutAnimation();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UComicSimulationWidget::OnFadeOutFinished, 0.5f, false);
}

void UComicSimulationWidget::OnFadeOutFinished()
{
	++CurrentIndex;

	if (ComicTextures.IsValidIndex(CurrentIndex))
	{
		SetImageTexture(ComicTextures[CurrentIndex]);
		PlayFadeInAnimation();

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UComicSimulationWidget::OnFadeInFinished, 0.5f, false);
	}
	else
	{
		bIsTransitioning = false;
	}
}

void UComicSimulationWidget::OnFadeInFinished()
{
	bIsTransitioning = false;
}

void UComicSimulationWidget::SetImageTexture(UTexture2D* Texture)
{
	if (Image && Texture)
	{
		Image->SetBrushFromTexture(Texture, true);
	}
}