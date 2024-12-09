// Fill out your copyright notice in the Description page of Project Settings.


#include "DownloadImageUI.h"
#include <Blueprint/AsyncTaskDownloadImage.h>
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/Texture2DDynamic.h"

void UDownloadImageUI::SetUrl(FString url)
{
	UAsyncTaskDownloadImage* async = UAsyncTaskDownloadImage::DownloadImage(url);
	async->OnSuccess.AddDynamic(this, &UDownloadImageUI::OnDownloadComplete);
	async->OnFail.AddDynamic(this, &UDownloadImageUI::OnDownloadComplete);
}

void UDownloadImageUI::OnDownloadComplete(UTexture2DDynamic* Texture)
{
	if (Texture)
	{
		downloadImage->SetBrushFromTextureDynamic(Texture);
		float ratio = 600.0f/ Texture->GetSurfaceWidth();
		downloadImage->SetBrushSize(FVector2D(Texture->GetSurfaceWidth(), Texture->GetSurfaceHeight() * ratio));

		/*UCanvasPanelSlot* panelSlot = Cast<UCanvasPanelSlot>(Slot);
		panelSlot->SetSize(FVector2D(Texture->GetSurfaceWidth(), Texture->GetSurfaceHeight()));*/
	}
	else
	{

	}
}
